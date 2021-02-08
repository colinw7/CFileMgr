#include <CFileMgrIcons.h>
#include <CFileMgr.h>
#include <CFileMgrDir.h>
#include <CFileMgrFile.h>

#include <CFileUtil.h>
#include <CImageLib.h>
#include <CPixelRenderer.h>
#include <CEvent.h>

CFileMgrIcons::
CFileMgrIcons(CFileMgr *file_mgr) :
 file_mgr_(file_mgr)
{
  CImage::setResizeType(CIMAGE_RESIZE_BILINEAR);
}

CFileMgrIcons::
~CFileMgrIcons()
{
}

void
CFileMgrIcons::
setRenderer(CPixelRenderer *renderer)
{
  renderer_ = renderer;

  updateFont();
}

void
CFileMgrIcons::
updateFont()
{
  auto font = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, file_mgr_->getFontSize());

  if (renderer_)
    renderer_->setFont(font);
}

void
CFileMgrIcons::
redraw()
{
  file_mgr_->redraw();
}

void
CFileMgrIcons::
draw()
{
  if (! renderer_)
    return;

  //-----

  CFileMgrDir::FileList file_list;

  file_mgr_->getFileList(file_list);

  //-----

  // Draw Icons
  int x = 0, y = 0;

  auto p1 = file_list.begin();
  auto p2 = file_list.end  ();

  for ( ; p1 != p2; ++p1)
    stepPosition(&x, &y);

  if (x > 0)
    y += file_mgr_->getIconHeight();

  size_.height = y;

  //-----

  renderer_->clear();

  //-----

  CIPoint2D p(0, 0);

  renderer_->applyOffset(p);

  p1 = file_list.begin();

  for ( ; p1 != p2; ++p1) {
    if ((*p1)->isDirectory())
      file_mgr_->drawDir(renderer_, p.x, p.y, *p1);
    else
      file_mgr_->drawFile(renderer_, p.x, p.y, *p1);

    stepPosition(&p.x, &p.y);
  }
}

void
CFileMgrIcons::
stepPosition(int *x, int *y)
{
  if (! renderer_)
    return;

  *x += file_mgr_->getIconWidth();

  if (*x + file_mgr_->getIconWidth() >= (int) renderer_->getWidth()) {
    *x  = 0;
    *y += file_mgr_->getIconHeight();
  }
}

void
CFileMgrIcons::
keyPress(const CKeyEvent &event)
{
  const std::string &str = event.getText();

  if      (str == "n")
    file_mgr_->selectNext();
  else if (str == "p")
    file_mgr_->selectPrev();

  redraw();
}

void
CFileMgrIcons::
keyRelease(const CKeyEvent &)
{
}
