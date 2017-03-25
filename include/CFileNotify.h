#ifndef CFILE_NOTIFY_H
#define CFILE_NOTIFY_H

#include <sys/inotify.h>

#include <string>
#include <list>
#include <map>

class CFileNotifier {
 public:
  CFileNotifier(const char *filename, uint flags=IN_ALL_EVENTS) :
   filename_(filename), flags_(flags), wd_(-1) {
  }

  virtual ~CFileNotifier() { }

  const std::string &getFilename() const { return filename_; }

  uint getFlags() const { return flags_; }

  void setWd(uint wd) { wd_ = wd; }

  int getWd() const { return wd_; }

  virtual void exec(uint flags) = 0;

 protected:
  std::string filename_;
  uint        flags_;
  int         wd_;
};

#define CFileNotifyMgrInst CFileNotifyMgr::getInstance()

class CFileNotifyMgr {
 public:
  static CFileNotifyMgr *getInstance();

  void addNotifier   (CFileNotifier *notifier);
  void removeNotifier(CFileNotifier *notifier);

  void update(uint maxusecs);

  static std::string flagsToString(uint flags);

 private:
  CFileNotifyMgr();
 ~CFileNotifyMgr();

 private:
  CFileNotifyMgr(const CFileNotifyMgr &);
  CFileNotifyMgr &operator=(const CFileNotifyMgr &);

  void execNotifiers(struct inotify_event *e);

 private:
  typedef std::list<CFileNotifier *> NotifierList;
  typedef std::map<int,NotifierList> FileNotifierList;

  int              notify_fd_;
  FileNotifierList notifiers_;
};

#endif
