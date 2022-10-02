#include <CFileMgrDetails.h>
#include <CFileMgr.h>
#include <CFileMgrDir.h>
#include <CFileMgrFile.h>
#include <CPixelRenderer.h>
#include <CImageLib.h>

CFileMgrDetails::
CFileMgrDetails(CFileMgr *file_mgr) :
 file_mgr_(file_mgr)
{
  columns_.push_back(CFileMgrColumnData("Name", CHALIGN_TYPE_LEFT ));
  columns_.push_back(CFileMgrColumnData("Perm", CHALIGN_TYPE_LEFT ));
  columns_.push_back(CFileMgrColumnData("Size", CHALIGN_TYPE_RIGHT));
  columns_.push_back(CFileMgrColumnData("Date", CHALIGN_TYPE_LEFT ));
  columns_.push_back(CFileMgrColumnData("Type", CHALIGN_TYPE_LEFT ));

  setColumns(columns_);
}

CFileMgrDetails::
~CFileMgrDetails()
{
}

void
CFileMgrDetails::
setRenderer(CPixelRenderer *renderer)
{
  renderer_ = renderer;

  updateFont();
}

void
CFileMgrDetails::
updateFont()
{
  auto font = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, file_mgr_->getFontSize());

  if (renderer_)
    renderer_->setFont(font);
}

void
CFileMgrDetails::
redraw()
{
  file_mgr_->redraw();
}

void
CFileMgrDetails::
draw()
{
}

void
CFileMgrDetails::
loadDir()
{
  setColumns(columns_);

  auto p1 = file_mgr_->fileBegin();
  auto p2 = file_mgr_->fileEnd  ();

  deleteAllRows();

  for (; p1 != p2; ++p1)
    (*p1)->setRow(0);

  int row = 1;

  p1 = file_mgr_->fileBegin();

  for (; p1 != p2; ++p1) {
    if (! (*p1)->isDirectory())
      continue;

    if (! file_mgr_->getShowHidden()) {
      if (! (*p1)->isParentDir() && (*p1)->isHidden())
        continue;
    }

    addRow(*p1);

    (*p1)->setRow(row++);
  }

  p1 = file_mgr_->fileBegin();

  for (; p1 != p2; ++p1) {
    if ((*p1)->isDirectory())
      continue;

    if (! file_mgr_->getShowHidden() && (*p1)->isHidden())
      continue;

    addRow(*p1);

    (*p1)->setRow(row++);
  }
}

void
CFileMgrDetails::
selectRowNotify(int)
{
}

void
CFileMgrDetails::
selectColumnNotify(int)
{
}

void
CFileMgrDetails::
activateRowNotify(int row)
{
  CFileMgrFile *file = rowToFile(row);

  activate(file);
}

void
CFileMgrDetails::
activate(CFileMgrFile *file)
{
  if (file == 0)
    return;

  if (file->isDirectory())
    file_mgr_->activateDir(file->getPathName());
  else
    file_mgr_->activateFile(file->getPathName());
}

void
CFileMgrDetails::
activateColumnNotify(int)
{
}

CFileMgrFile *
CFileMgrDetails::
rowToFile(int row)
{
  CFileMgrDir::file_iterator p1 = file_mgr_->fileBegin();
  CFileMgrDir::file_iterator p2 = file_mgr_->fileEnd  ();

  for ( ; p1 != p2; ++p1)
    if ((*p1)->getRow() == row)
      return *p1;

  return 0;
}

void
CFileMgrDetails::
setColumns(const CFileMgrDetails::ColumnList &columns)
{
  file_mgr_->setTableColumns(columns);
}

void
CFileMgrDetails::
deleteAllRows()
{
  file_mgr_->deleteAllTableRows();
}

void
CFileMgrDetails::
addRow(CFileMgrFile *file)
{
  std::vector<std::string> values;

  auto num_values = columns_.size();

  for (uint i = 0; i < num_values; ++i)
    values.push_back(file->getValue(columns_[i].name));

  file_mgr_->addTableRow(file, values);
}

void
CFileMgrDetails::
getScrollLimits(double *start, double *end)
{
  *start = scroll_start_;
  *end   = scroll_end_;
}

void
CFileMgrDetails::
setScrollLimits(double start, double end)
{
  scroll_start_ = start;
  scroll_end_   = end;
}

void
CFileMgrDetails::
updateScrollLimits()
{
  double start, end;

  getScrollLimits(&start, &end);

  start -= start;
  end   -= start;

  //-----

  setOffset(start);

  int y1 = int(start*getRowsHeight());
  int y2 = y1 + renderer_->getHeight();

  setScrollLimits(start, (1.0*y2)/getRowsHeight());
}
