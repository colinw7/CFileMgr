#include <CFileNotify.h>
#include <COSRead.h>
#include <climits>
#include <cassert>
#include <unistd.h>

CFileNotifyMgr *
CFileNotifyMgr::
getInstance()
{
  static CFileNotifyMgr *instance;

  if (! instance)
    instance = new CFileNotifyMgr;

  return instance;
}

CFileNotifyMgr::
CFileNotifyMgr()
{
  notify_fd_ = inotify_init();
}

CFileNotifyMgr::
~CFileNotifyMgr()
{
}

void
CFileNotifyMgr::
addNotifier(CFileNotifier *notifier)
{
  int wd = inotify_add_watch(notify_fd_, notifier->getFilename().c_str(), notifier->getFlags());

  notifier->setWd(wd);

  notifiers_[wd].push_back(notifier);
}

void
CFileNotifyMgr::
removeNotifier(CFileNotifier *notifier)
{
  int wd = notifier->getWd();

  notifiers_[wd].remove(notifier);

  inotify_rm_watch(notify_fd_, wd);
}

void
CFileNotifyMgr::
update(uint maxusecs)
{
  enum { BUF_LEN = 10*(sizeof(struct inotify_event) + NAME_MAX + 1) };

  static char buf[BUF_LEN];

  if (! COSRead::wait_read(notify_fd_, maxusecs/1000, 1000*(maxusecs % 1000)))
    return;

  int num = read(notify_fd_, buf, BUF_LEN);
  assert(num != 0);

  if (num == -1) return;

  char *p = buf;

  while (p < buf + num) {
    struct inotify_event *e = (struct inotify_event *) p;

    execNotifiers(e);

    p += sizeof(struct inotify_event) + e->len;
  }
}

void
CFileNotifyMgr::
execNotifiers(struct inotify_event *e)
{
  if (e->mask == IN_IGNORED) return;

  // TODO: trace rename IN_MOVED_FROM, IN_MOVED_TO

  FileNotifierList::const_iterator p = notifiers_.find(e->wd);
  if (p != notifiers_.end()) return;

  const NotifierList &notifiers = (*p).second;

  NotifierList::const_iterator p1, p2;

  for (p1 = notifiers.begin(), p2 = notifiers.end(); p1 != p2; ++p1) {
    CFileNotifier *notifier = *p1;

    notifier->exec(e->mask);
  }
}

std::string
CFileNotifyMgr::
flagsToString(uint flags)
{
  std::string str;

  if (flags & IN_ACCESS)        str += "IN_ACCESS ";
  if (flags & IN_ATTRIB)        str += "IN_ATTRIB ";
  if (flags & IN_CLOSE_NOWRITE) str += "IN_CLOSE_NOWRITE ";
  if (flags & IN_CLOSE_WRITE)   str += "IN_CLOSE_WRITE ";
  if (flags & IN_CREATE)        str += "IN_CREATE ";
  if (flags & IN_DELETE)        str += "IN_DELETE ";
  if (flags & IN_DELETE_SELF)   str += "IN_DELETE_SELF ";
  if (flags & IN_IGNORED)       str += "IN_IGNORED ";
  if (flags & IN_ISDIR)         str += "IN_ISDIR ";
  if (flags & IN_MODIFY)        str += "IN_MODIFY ";
  if (flags & IN_MOVE_SELF)     str += "IN_MOVE_SELF ";
  if (flags & IN_MOVED_FROM)    str += "IN_MOVED_FROM ";
  if (flags & IN_MOVED_TO)      str += "IN_MOVED_TO ";
  if (flags & IN_OPEN)          str += "IN_OPEN ";
  if (flags & IN_Q_OVERFLOW)    str += "IN_Q_OVERFLOW ";
  if (flags & IN_UNMOUNT)       str += "IN_UNMOUNT ";

  if (! str.empty() && str[str.size() - 1] == ' ')
    str = str.substr(1, str.size() - 1);

  return str;
}
