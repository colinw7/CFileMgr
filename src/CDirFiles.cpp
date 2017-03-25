#include <CDirFiles.h>
#include <CDir.h>
#include <CDirScope.h>

#include <algorithm>

CDirFiles::
CDirFiles(CDir *dir)
{
  if (dir == NULL) {
    dir_   = new CDir(CDir::getCurrent());
    owner_ = true;
  }
  else {
    dir_   = dir;
    owner_ = false;
  }
}

CDirFiles::
CDirFiles(const char *dirname)
{
  dir_   = new CDir(dirname);
  owner_ = true;
}

CDirFiles::
~CDirFiles()
{
  for (int i = 0; i < size(); i++)
    delete files_[i];

  if (owner_)
    delete dir_;
}

bool
CDirFiles::
addFilename(const std::string &filename)
{
  CDirScope dir_scope(*dir_);

  if (! dir_scope.valid())
    return false;

  addDirFilename(filename);

  return true;
}

void
CDirFiles::
addDirFilename(const std::string &filename)
{
  files_.push_back(new CFile(filename));
}

CFile *
CDirFiles::
getFile(int pos)
{
  if (pos < 0 || pos > size()) {
    error_msg_ = "Invalid Subscript";

    return NULL;
  }

  return files_[pos];
}

void
CDirFiles::
sort()
{
  std::sort(begin(), end(), CDirFilesCmp());
}

//------

bool
CDirFiles::CDirFilesCmp::
operator()(CFile *a, CFile *b)
{
  return (a->getName() < b->getName());
}
