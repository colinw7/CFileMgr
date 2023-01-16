#ifndef CQFileMgrDetails_H
#define CQFileMgrDetails_H

#include <CFileMgrDetails.h>
#include <QTreeWidget>

class CQFileMgr;
class CFileMgrFile;
class QContextMenuEvent;

class CQFileMgrDetails : public QTreeWidget {
  Q_OBJECT

 public:
  CQFileMgrDetails(QWidget *parent, CQFileMgr *filemgr);

  void setColumns(const CFileMgrDetails::ColumnList &names);

  void addRow(CFileMgrFile *file, const std::vector<std::string> &values);

 private:
  void contextMenuEvent(QContextMenuEvent *event) override;

 private slots:
  void selectionChanged();
  void activateItem();
  void iconsView();
  void filmView();

 private:
  using QTreeWidget::selectionChanged;

 private:
  typedef std::vector<int> Aligns;

  CQFileMgr *filemgr_ { nullptr };
  Aligns     aligns_;
};

#endif
