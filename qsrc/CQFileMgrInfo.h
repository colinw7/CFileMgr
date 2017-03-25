#ifndef CQFILEMGR_INFO_H
#define CQFILEMGR_INFO_H

#include <QDialog>

class CQFileMgr;
class QLineEdit;

class CQFileMgrInfo : public QDialog {
  Q_OBJECT

 public:
  CQFileMgrInfo(CQFileMgr *mgr);
 ~CQFileMgrInfo();

  void init(const std::string &fileName);

 private:
  CQFileMgr *mgr_ { nullptr };
  QLineEdit *nedit_ { nullptr };
};

#endif
