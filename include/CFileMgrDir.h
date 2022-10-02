#ifndef CFILE_MGR_DIR_H
#define CFILE_MGR_DIR_H

#include <string>
#include <vector>

class CFileMgr;
class CFileMgrFile;

class CFileMgrDir {
 public:
  typedef std::vector<CFileMgrFile *> FileList;
  typedef FileList::iterator          file_iterator;

 public:
  CFileMgrDir(CFileMgr *file_mgr);

  CFileMgr *getFileMgr() const { return file_mgr_; }

  const std::string &getDirName() const { return dirname_; }

  void setDirName(const std::string &dirname) { dirname_ = dirname; }

  uint numFiles() { return uint(file_list_.size()); }

  file_iterator fileBegin() { return file_list_.begin(); }
  file_iterator fileEnd  () { return file_list_.end  (); }

  CFileMgrFile *getFile(int i) { return file_list_[i]; }

  bool reload(const std::string &dirname, bool force=false);

  void deselectAll();

  FileList getSelected();

  CFileMgrFile *getCurrentFile();

  void nextFile();
  void prevFile();

 private:
  CFileMgr    *file_mgr_ { nullptr };
  std::string  dirname_;
  FileList     file_list_;
  uint         current_num_ { 0 };
};

#endif
