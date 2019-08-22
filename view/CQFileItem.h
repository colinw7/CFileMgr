#ifndef CQFileItem_H
#define CQFileItem_H

#include <QList>
#include <QString>
#include <QVariant>

#include <string>

class CQFileItem {
 public:
  CQFileItem(const std::string &filename);
  CQFileItem(CQFileItem *parent, const std::string &filename);

 ~CQFileItem();

  const std::string &filename() const { return filename_; }

  CQFileItem *parent() { return parentItem_; }

  bool isRoot() const { return filename_ == ""; }

  void clear();

  void appendChild(CQFileItem *child);

  CQFileItem *child(int row);

  int childCount() const;

  int row() const;

  CQFileItem *findChild(const std::string &filename);

  QVariant data(const QString &key) const;

  void setData(const QString &key, const QVariant &data);

 private:
  void init();

 private:
  typedef QList<CQFileItem*>     FileItems;
  typedef QMap<QString,QVariant> DataMap;

  std::string  filename_;
  CQFileItem  *parentItem_;
  FileItems    childItems_;
  DataMap      data_;
};

#endif
