#ifndef CDirFilesMgr_H
#define CDirFilesMgr_H

#include <CDirFiles.h>

class CDirFilesMgr {
 public:
  CDirFilesMgr(CDir *dir);
 ~CDirFilesMgr();

  CDirFiles *getSortedFiles();

  bool getSortedFiles(CDirFiles **dir_files);

  CDirFiles *getFiles();

  bool getFiles(CDirFiles **dir_files);

  typedef CDirFiles::iterator iterator;

  iterator begin() {
    if (dir_files_ == NULL)
      dir_files_ = getFiles();

    return dir_files_->begin();
  }

  iterator end() {
    if (dir_files_ == NULL)
      dir_files_ = getFiles();

    return dir_files_->end();
  }

  CFile *operator[](int pos) {
    if (dir_files_ == NULL)
      dir_files_ = getFiles();

    return dir_files_->getFile(pos);
  }

 private:
  bool getSortedFilesI(CDirFiles **dir_files);

  bool getFilesI(CDirFiles **dir_files);

 private:
  CDir      *dir_ { nullptr };
  CDirFiles *dir_files_ { nullptr };
};

#endif
