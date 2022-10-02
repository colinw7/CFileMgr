#include <CFileMgrDir.h>
#include <CFileMgr.h>
#include <CFileMgrFile.h>

#include <CStrUtil.h>
#include <CDir.h>
#include <CFileUtil.h>
#include <CTime.h>
#include <CFuncs.h>

CFileMgrDir::
CFileMgrDir(CFileMgr *file_mgr) :
 file_mgr_(file_mgr)
{
}

bool
CFileMgrDir::
reload(const std::string &dirname, bool force)
{
  if (! force && (dirname == getDirName()))
    return false;

  setDirName(dirname);

  CDir::enter(dirname);

  file_mgr_->changeDir(CDir::getCurrent());

  CDir dir(".");

  std::vector<std::string> files;

  dir.getSortedFilenames(files);

  for (auto &f : file_list_)
    delete f;

  file_list_.clear();

  if (file_mgr_->getShowDotDot()) {
    CFileMgrFile *file = new CFileMgrFile(this, "..");

    file_list_.push_back(file);
  }

  auto num_files = files.size();

  for (uint i = 0; i < num_files; ++i) {
    CFileMgrFile *file = new CFileMgrFile(this, files[i]);

    file_list_.push_back(file);
  }

  return true;
}

void
CFileMgrDir::
deselectAll()
{
  CFileMgrDir::file_iterator p1 = fileBegin();
  CFileMgrDir::file_iterator p2 = fileEnd  ();

  for ( ; p1 != p2; ++p1)
    (*p1)->setSelected(false);
}

CFileMgrDir::FileList
CFileMgrDir::
getSelected()
{
  FileList selected;

  CFileMgrDir::file_iterator p1 = fileBegin();
  CFileMgrDir::file_iterator p2 = fileEnd  ();

  for ( ; p1 != p2; ++p1)
    if ((*p1)->getSelected())
      selected.push_back(*p1);

  return selected;
}

CFileMgrFile *
CFileMgrDir::
getCurrentFile()
{
  uint num = 0;

  CFileMgrDir::file_iterator p1 = fileBegin();
  CFileMgrDir::file_iterator p2 = fileEnd  ();

  for ( ; p1 != p2; ++p1) {
    if ((*p1)->isDirectory())
      continue;

    if (num == current_num_)
      return *p1;

    ++num;
  }

  current_num_ = num;

  return 0;
}

void
CFileMgrDir::
nextFile()
{
  ++current_num_;
}

void
CFileMgrDir::
prevFile()
{
  --current_num_;
}
