#ifndef CDirFiles_H
#define CDirFiles_H

#include <string>
#include <vector>

class CFile;
class CDir;

class CDirFiles {
 private:
  class CDirFilesCmp {
   public:
    bool operator()(CFile *a, CFile *b);
  };

 public:
  typedef std::vector<CFile *>::iterator iterator;

  explicit CDirFiles(CDir *dir=0);

  CDirFiles(const char *dirname);

 ~CDirFiles();

  uint size() { return uint(files_.size()); }

  bool addFilename(const std::string &filename);
  void addDirFilename(const std::string &filename);

  CFile *getFile(int pos);

  void sort();

  iterator begin() {
    return files_.begin();
  }

  iterator end() {
    return files_.end();
  }

  CFile *operator[](int pos) {
    return getFile(pos);
  }

  const std::string &getErrorMsg() const { return error_msg_; }

 private:
  std::string error_msg_;

  CDir *               dir_ { nullptr };
  bool                 owner_ { false };
  std::vector<CFile *> files_;
};

#endif
