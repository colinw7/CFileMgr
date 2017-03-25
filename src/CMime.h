#ifndef CMIME_H
#define CMIME_H

#include <string>

#define CMimeInst CMime::getInstancePtr()

#include <CSingleton.h>
#include <CFileType.h>

class CConfig;

class CMime : public CSingleton<CMime> {
 protected:
  CMime() : config_(NULL) { }

 public:
  bool getOpenFileCommand(const std::string &filename, std::string &cmd);
  bool getOpenFileCommand(const std::string &filename, CFileType type, std::string &cmd);

  bool getFileCommand(const std::string &filename, CFileType type,
                      const std::string &op, std::string &cmd);

  bool getApplication(CFileType type, const std::string &name, std::string &appname);

  std::string getFileType(const std::string &filename);

 private:
  CConfig *getConfig();

 private:
  friend class CSingleton<CMime>;

  CConfig *config_;
};

#endif
