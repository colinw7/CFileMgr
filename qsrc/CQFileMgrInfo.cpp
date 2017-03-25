#include <CQFileMgrInfo.h>
#include <CQFileMgr.h>

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

CQFileMgrInfo::
CQFileMgrInfo(CQFileMgr *mgr) :
 QDialog(mgr), mgr_(mgr)
{
  setObjectName("fileMgrInfo");

  QGridLayout *grid = new QGridLayout(this);

  QLabel *nlabel = new QLabel("Name");

  nedit_ = new QLineEdit;

  grid->addWidget(nlabel, 0, 0);
  grid->addWidget(nedit_, 0, 1);
}

CQFileMgrInfo::
~CQFileMgrInfo()
{
}

void
CQFileMgrInfo::
init(const std::string &fileName)
{
  CFile file(fileName);

  nedit_->setText(file.getName().c_str());
}
