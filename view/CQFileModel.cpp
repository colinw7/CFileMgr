#include <CQFileModel.h>
#include <CQFileItem.h>

#include <CFile/CDir.h>

#include <QtGui>

// When subclassing QAbstractItemModel, at the very least you must implement index(),
// parent(), rowCount(), columnCount(), and data(). These functions are used in all
// read-only models, and form the basis of editable models
//
// You can also reimplement hasChildren() to provide special behavior for models where
// the implementation of rowCount() is expensive. This makes it possible for models to
// restrict the amount of data requested by views, and can be used as a way to implement
// lazy population of model data.
//
// Custom models need to create model indexes for other components to use. To do this,
// call createIndex() with suitable row and column numbers for the item, and an identifier
// for it, either as a pointer or as an integer value. The combination of these values
// must be unique for each item. Custom models typically use these unique identifiers in
// other reimplemented functions to retrieve item data and access information about the
// item's parents and children.
//
// It is not necessary to support every role defined in Qt::ItemDataRole. Depending on the
// type of data contained within a model, it may only be useful to implement the data()
// function to return valid information for some of the more common roles. Most models
// provide at least a textual representation of item data for the Qt::DisplayRole, and
// well-behaved models should also provide valid information for the Qt::ToolTipRole and
// Qt::WhatsThisRole. Supporting these roles enables models to be used with standard Qt views.
// However, for some models that handle highly-specialized data, it may be appropriate to
// provide data only for user-defined roles.
//
// Models that provide interfaces to resizable data structures can provide implementations of
// insertRows(), removeRows(), insertColumns(),and removeColumns(). When implementing these
// functions, it is important to notify any connected views about changes to the model's
// dimensions both before and after they occur:
// . An insertRows() implementation must call beginInsertRows() before inserting new rows
//   into the data structure, and endInsertRows() immediately afterwards.
// . An insertColumns() implementation must call beginInsertColumns() before inserting new
//   columns into the data structure, and endInsertColumns() immediately afterwards.
// . A removeRows() implementation must call beginRemoveRows() before the rows are removed
//   from the data structure, and endRemoveRows() immediately afterwards.
// . A removeColumns() implementation must call beginRemoveColumns() before the columns are
//   removed from the data structure, and endRemoveColumns() immediately afterwards.
//
// The private signals that these functions emit give attached components the chance to take
// action before any data becomes unavailable. The encapsulation of the insert and remove
// operations with these begin and end functions also enables the model to manage persistent
// model indexes correctly. If you want selections to be handled properly, you must ensure
// that you call these functions. If you insert or remove an item with children, you do not
// need to call these functions for the child items. In other words, the parent item will
// take care of its child items.
//
// To create models that populate incrementally, you can reimplement fetchMore() and
// canFetchMore(). If the reimplementation of fetchMore() adds rows to the model,
// beginInsertRows() and endInsertRows() must be called.

CQFileModel::
CQFileModel(QObject *parent) :
 QAbstractItemModel(parent), columns_(0)
{
  rootItem_ = new CQFileItem("");

  headerFont_.setBold(true);
}

CQFileModel::
~CQFileModel()
{
  delete rootItem_;
}

// specify columns for the table
void
CQFileModel::
setColumns(CQFileColumns *columns)
{
  columns_ = columns;
}

// add files in directory
void
CQFileModel::
addDirFiles(const std::string &dirname, CQFileVisitor *visitor)
{
  root()->setData("Name", QVariant(QString(dirname.c_str())));

  addDirFiles(root(), dirname, visitor);
}

void
CQFileModel::
addDirFiles(CQFileItem *file, const std::string &dirname, CQFileVisitor *visitor)
{
  if (! CDir::enter(dirname))
    return;

  if (visitor)
    visitor->enterDir(file);

  std::vector<std::string> files;

  CFile::setUseLStat(true);

  CDir::getFilenames(".", files);

  uint n = files.size();

  for (uint i = 0; i < n; ++i) {
    const std::string &filename = files[i];

    CQFileItem *file1 = addFile(file, filename);

    if (visitor)
      visitor->visitFile(file1);

    if (CFile::isDirectory(filename))
      addDirFiles(file1, filename, visitor);
  }

  if (visitor)
    visitor->leaveDir(file);

  CDir::leave();
}

// add item to the tree
CQFileItem *
CQFileModel::
addFile(const std::string &filename)
{
  CQFileItem *item = new CQFileItem(rootItem_, filename);

  rootItem_->appendChild(item);

  return item;
}

// add item to the tree
CQFileItem *
CQFileModel::
addFile(CQFileItem *parent, const std::string &filename)
{
  CQFileItem *item = new CQFileItem(parent, filename);

  parent->appendChild(item);

  return item;
}

// find id in tree
CQFileItem *
CQFileModel::
findItem(const std::string &filename)
{
  return rootItem_->findChild(filename);
}

// get number of columns for row (child or top level)
int
CQFileModel::
columnCount(const QModelIndex &) const
{
  if (columns_)
    return columns_->count();
  else
    return 1;
}

// get data for row/column
QVariant
CQFileModel::
data(const QModelIndex &index, int role) const
{
  if (! index.isValid())
    return QVariant();

  // TODO: other roles
  if      (role == Qt::DisplayRole) {
    QString key = (columns_ ? columns_->key(index.column()) : "name");

    CQFileItem *item = static_cast<CQFileItem *>(index.internalPointer());

    if (item->isRoot())
      return key;
    else
      return item->data(key);
  }
  else if (role == Qt::ToolTipRole || role == Qt::WhatsThisRole) {
    QString key = (columns_ ? columns_->key(index.column()) : "name");

    CQFileItem *item = static_cast<CQFileItem *>(index.internalPointer());

    if (item->isRoot())
      return key;
    else
      return item->data(key);
  }
  else if (role == Qt::TextColorRole) {
    if (index.column() == 0)
      return QColor(20,20,200);
    else
      return QColor(0,0,0);
  }
  else if (role == Qt::DecorationRole) {
    return QVariant();
  }
  else
    return QVariant();
}

// get properties for row/column
Qt::ItemFlags
CQFileModel::
flags(const QModelIndex &index) const
{
  if (! index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

// get header for column
QVariant
CQFileModel::
headerData(int section, Qt::Orientation /*orientation*/, int role) const
{
  if      (role == Qt::DisplayRole) {
    if (columns_) {
      if (section >= 0 && section <= columns_->count())
        return columns_->key(section);
    }

    return QVariant();
  }
  else if (role == Qt::FontRole)
    return headerFont_;
  else
    return QVariant();
}

// create index for element at specified row/column and parent
QModelIndex
CQFileModel::
index(int row, int column, const QModelIndex &parent) const
{
  if (! hasIndex(row, column, parent))
    return QModelIndex();

  CQFileItem *parentItem;

  if (! parent.isValid())
    parentItem = rootItem_;
  else
    parentItem = static_cast<CQFileItem *>(parent.internalPointer());

  CQFileItem *childItem = parentItem->child(row);

  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

// get index for parent of item at specified index
QModelIndex
CQFileModel::
parent(const QModelIndex &index) const
{
  if (! index.isValid())
    return QModelIndex();

  CQFileItem *childItem = static_cast<CQFileItem *>(index.internalPointer());

  CQFileItem *parentItem = childItem->parent();

  if (parentItem == rootItem_)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

// get number of rows for item at specified index
int
CQFileModel::
rowCount(const QModelIndex &parent) const
{
  CQFileItem *parentItem;

  if (parent.column() > 0)
    return 0;

  if (! parent.isValid())
    parentItem = rootItem_;
  else
    parentItem = static_cast<CQFileItem *>(parent.internalPointer());

  return parentItem->childCount();
}

QString
CQFileModel::
getData(const QModelIndex &index) const
{
  if (! index.isValid())
    return "";

  CQFileItem *item = static_cast<CQFileItem *>(index.internalPointer());

  if (item->isRoot()) {
    if (columns_)
      return columns_->key(index.column());
    else
      return "";
  }
  else {
    QString key = (columns_ ? columns_->key(index.column()) : "name");

    if (columns_)
      return item->data(key).toString();
    else
      return "";
  }
}

//-------

bool
CQFileModelSort::
lessThan(const QModelIndex &left, const QModelIndex &right) const
{
  QVariant leftData  = sourceModel()->data(left);
  QVariant rightData = sourceModel()->data(right);

  QVariant::Type type1 = leftData .type();
  QVariant::Type type2 = rightData.type();

  if (type1 == type2) {
    switch (type1) {
      case QVariant::Bool:
        return leftData.value<bool>() < rightData.value<bool>();
      case QVariant::Char:
        return leftData.value<QChar>() < rightData.value<QChar>();
      case QVariant::Int:
        return leftData.value<int>() < rightData.value<int>();
      case QVariant::LongLong:
        return leftData.value<qlonglong>() < rightData.value<qlonglong>();
      case QVariant::ULongLong:
        return leftData.value<qulonglong>() < rightData.value<qulonglong>();
      case QVariant::Date:
        return leftData.value<QDate>() < rightData.value<QDate>();
      case QVariant::Time:
        return leftData.value<QTime>() < rightData.value<QTime>();
      case QVariant::DateTime:
        return leftData.value<QDateTime>() < rightData.value<QDateTime>();
      default:
        break;
    }
  }

  QString leftString  = leftData .toString();
  QString rightString = rightData.toString();

  return leftString < rightString;
}
