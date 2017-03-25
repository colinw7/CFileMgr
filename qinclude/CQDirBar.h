#ifndef CQDIRBAR_H
#define CQDIRBAR_H

#include <QWidget>

class QHBoxLayout;

class CQFileMgr;
class CQDirBar;

class CQDirBarItem : public QWidget {
  Q_OBJECT

 public:
  CQDirBarItem(CQDirBar *bar, const QString &text="");
 ~CQDirBarItem();

  void setText(const QString &text);

  const QString &text() { return text_; }

  void setDir(const QString &dir);

  const QString &dir() const { return dir_; }

  void setFirst(bool first) { first_ = first; }
  void setLast (bool last ) { last_  = last ; }

  int getMinWidth();

  QSize sizeHint() const;
  QSize minimumSizeHint() const;

 private:
  void mousePressEvent(QMouseEvent *event);

  void paintEvent(QPaintEvent *event);

 private:
  CQDirBar *bar_        { 0 };
  QString   text_;
  QString   dir_;
  bool      first_      { false };
  bool      last_       { false };
  int       border_     { 4 };
  int       arrowWidth_ { 8 };
};

//------

class CQDirBar : public QWidget {
  Q_OBJECT

 public:
  CQDirBar(QWidget *parent=0);
 ~CQDirBar();

  QSize sizeHint() const;
  QSize minimumSizeHint() const;

 private:
  void setNumWidgets(uint num_widgets);

 private slots:
  void updateDir();

 private:
  typedef std::vector<CQDirBarItem *> WidgetList;

  QHBoxLayout *layout_      { 0 };
  WidgetList   widgets_;
  uint         num_widgets_ { 0 };
};

#endif
