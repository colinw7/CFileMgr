#include <CQFileMgrDetails.h>
#include <CQFileMgr.h>
#include <CFileMgrLib.h>
#include <CStrUtil.h>

#include <QTreeWidgetItem>
#include <QMenu>
#include <QContextMenuEvent>

class CQFileMgrDetailsRow : public QTreeWidgetItem {
 public:
  CQFileMgrDetailsRow(CQFileMgrDetails *details, CFileMgrFile *file, const QStringList &slist) :
   QTreeWidgetItem(slist), details_(details), file_(file) {
  }

  CFileMgrFile *getFile() {
    return file_;
  }

  bool operator<(const QTreeWidgetItem &other) const {
    int col = details_->sortColumn();

    std::string s1 =       text(col).toStdString();
    std::string s2 = other.text(col).toStdString();

    int i1, i2;

    if (CStrUtil::toInteger(s1, &i1) && CStrUtil::toInteger(s2, &i2))
      return (i1 < i2);

    double r1, r2;

    if (CStrUtil::toReal(s1, &r1) && CStrUtil::toReal(s2, &r2))
      return (r1 < r2);

    return (s1 < s2);
  }

 private:
  CQFileMgrDetails *details_ { nullptr };
  CFileMgrFile     *file_ { nullptr };
};

//------

CQFileMgrDetails::
CQFileMgrDetails(QWidget *parent, CQFileMgr *filemgr) :
 QTreeWidget(parent), filemgr_(filemgr)
{
  setObjectName("fileMgrDetails");

  setSortingEnabled(true);
  setRootIsDecorated(false);

  setSelectionMode(QAbstractItemView::ExtendedSelection);

  connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
  connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(activateItem()));
}

void
CQFileMgrDetails::
selectionChanged()
{
  QList<QTreeWidgetItem *> items = selectedItems();

  filemgr_->deselectAll();

  for (int i = 0; i < items.size(); ++i) {
    CQFileMgrDetailsRow *item = reinterpret_cast<CQFileMgrDetailsRow *>(items.at(i));

    item->getFile()->setSelected(true);
  }

  filemgr_->expose();
}

void
CQFileMgrDetails::
activateItem()
{
  CQFileMgrDetailsRow *item = reinterpret_cast<CQFileMgrDetailsRow *>(currentItem());

  filemgr_->activate(item->getFile());
}

void
CQFileMgrDetails::
setColumns(const CFileMgrDetails::ColumnList &columns)
{
  uint num_columns = columns.size();

  setColumnCount(num_columns);

  QStringList slist;

  for (uint i = 0; i < num_columns; ++i)
    slist << QString(columns[i].name.c_str());

  setHeaderLabels(slist);

  aligns_.clear();

  for (uint i = 0; i < num_columns; ++i) {
    int align = columns[i].align == CHALIGN_TYPE_LEFT ?  Qt::AlignLeft : Qt::AlignRight;

    aligns_.push_back(align);
  }
}

void
CQFileMgrDetails::
addRow(CFileMgrFile *file, const std::vector<std::string> &values)
{
  QStringList slist;

  uint num_values = values.size();

  for (uint i = 0; i < num_values; ++i)
    slist << QString(values[i].c_str());

  CQFileMgrDetailsRow *item = new CQFileMgrDetailsRow(this, file, slist);

  addTopLevelItem(item);

  for (uint i = 0; i < num_values; ++i)
    item->setTextAlignment(i, aligns_[i]);
}

void
CQFileMgrDetails::
contextMenuEvent(QContextMenuEvent *event)
{
  static QAction *iconsAction;
  static QAction *filmAction;

  if (! iconsAction) {
    iconsAction = new QAction(tr("&Icons View"    ), this);
    filmAction  = new QAction(tr("&Filmstrip View"), this);

    connect(iconsAction, SIGNAL(triggered()), this, SLOT(iconsView()));
    connect(filmAction, SIGNAL(triggered()), this, SLOT(filmView()));
  }

  QMenu menu(this);

  menu.addAction(iconsAction);
  menu.addAction(filmAction);

  menu.exec(event->globalPos());
}

void
CQFileMgrDetails::
iconsView()
{
  filemgr_->setIconsView();
}

void
CQFileMgrDetails::
filmView()
{
  filemgr_->setFilmstripView();
}
