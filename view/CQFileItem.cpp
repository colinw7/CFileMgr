#include <CQFileView/CQFileItem.h>

#include <sys/stat.h>
#include <cstdio>

CQFileItem::
CQFileItem(const std::string &filename) :
 filename_(filename), parentItem_(0), childItems_()
{
  init();
}

CQFileItem::
CQFileItem(CQFileItem *parent, const std::string &filename) :
 filename_(filename), parentItem_(parent), childItems_()
{
  init();
}

CQFileItem::
~CQFileItem()
{
  qDeleteAll(childItems_);
}

void
CQFileItem::
init()
{
  setData("Name", QVariant(QString(filename_.c_str())));
}

void
CQFileItem::
clear()
{
  qDeleteAll(childItems_);

  childItems_.clear();
}

void
CQFileItem::
appendChild(CQFileItem *item)
{
  childItems_.append(item);
}

CQFileItem *
CQFileItem::
child(int row)
{
  return childItems_.value(row);
}

int
CQFileItem::
childCount() const
{
  return childItems_.count();
}

int
CQFileItem::
row() const
{
  if (parentItem_)
    return parentItem_->childItems_.indexOf(const_cast<CQFileItem *>(this));

  return 0;
}

CQFileItem *
CQFileItem::
findChild(const std::string &filename)
{
  for (int i = 0; i < childItems_.count(); ++i)
    if (filename == childItems_[i]->filename())
      return childItems_[i];

  return 0;
}

QVariant
CQFileItem::
data(const QString &key) const
{
  if (data_.contains(key))
    return data_[key];
  else
    return QVariant();
}

void
CQFileItem::
setData(const QString &key, const QVariant &data)
{
  data_[key] = data;
}
