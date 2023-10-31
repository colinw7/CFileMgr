#include <CDirFilesMgr.h>
#include <CDir.h>

struct CDirProcessGetFilesProc : public CDirProcessProc {
  CDirFiles *dir_files;

  CDirProcessGetFilesProc(CDirFiles *dir_files1) : dir_files(dir_files1) { }

  void process() override { dir_files->addDirFilename(filename); }
};

CDirFilesMgr::
CDirFilesMgr(CDir *dir) :
 dir_(dir)
{
}

CDirFilesMgr::
~CDirFilesMgr()
{
  delete dir_files_;
}

CDirFiles *
CDirFilesMgr::
getSortedFiles()
{
  if (! getSortedFilesI(&dir_files_))
    return NULL;

  return dir_files_;
}

bool
CDirFilesMgr::
getSortedFiles(CDirFiles **dir_files)
{
  *dir_files = nullptr;

  return getSortedFilesI(dir_files);
}

bool
CDirFilesMgr::
getSortedFilesI(CDirFiles **dir_files)
{
  if (! getFilesI(dir_files))
    return false;

  (*dir_files)->sort();

  return true;
}

CDirFiles *
CDirFilesMgr::
getFiles()
{
  if (! getFilesI(&dir_files_))
    return NULL;

  return dir_files_;
}

bool
CDirFilesMgr::
getFiles(CDirFiles **dir_files)
{
  *dir_files = nullptr;

  return getFilesI(dir_files);
}

bool
CDirFilesMgr::
getFilesI(CDirFiles **dir_files)
{
  if (! dir_files)
    *dir_files = new CDirFiles(dir_);

  CDirProcessGetFilesProc get_files_proc(*dir_files);

  if (! dir_->processFiles(get_files_proc)) {
    delete *dir_files;

    *dir_files = NULL;

    return false;
  }

  (*dir_files)->sort();

  return true;
}
