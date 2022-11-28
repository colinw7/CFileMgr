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

  const QString &text() const { return text_; }
  void setText(const QString &text);

  const QString &dir() const { return dir_; }
  void setDir(const QString &dir);

  void setFirst(bool first) { first_ = first; }
  void setLast (bool last ) { last_  = last ; }

  int getMinWidth();

  int border() const { return border_; }
  void setBorder(int b) { border_ = b; }

  int arrowWidth() const { return arrowWidth_; }
  void setArrowWidth(int w) { arrowWidth_ = w; }

  bool isClipped() const { return clipped_; }
  void setClipped(bool b) { clipped_ = b; }

  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;

 private:
  void mousePressEvent(QMouseEvent *event) override;

  void paintEvent(QPaintEvent *event) override;

 private:
  CQDirBar *bar_        { 0 };
  QString   text_;
  QString   dir_;
  bool      first_      { false };
  bool      last_       { false };
  bool      clipped_    { false };
  int       border_     { 4 };
  int       arrowWidth_ { 8 };
};

//------

class CQDirBar : public QWidget {
  Q_OBJECT

  Q_PROPERTY(int arrowWidth READ arrowWidth WRITE setArrowWidth)
  Q_PROPERTY(int border     READ border     WRITE setBorder    )

 public:
  CQDirBar(QWidget *parent=0);
 ~CQDirBar();

  int border() const { return border_; }
  void setBorder(int b);

  int arrowWidth() const { return arrowWidth_; }
  void setArrowWidth(int w);

  void paintEvent(QPaintEvent *) override;

  void resizeEvent(QResizeEvent *) override;

  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;

 private:
  void setNumWidgets(uint num_widgets);

  void updateSizes();

 private Q_SLOTS:
  void fontChanged();
  void updateDir();

 private:
  typedef std::vector<CQDirBarItem *> WidgetList;

//QHBoxLayout *layout_       { 0 };
  WidgetList   widgets_;
  uint         num_widgets_  { 0 };
  int          border_       { 4 };
  int          arrowWidth_   { 8 };
  bool         clipped_      { false };
  int          clippedWidth_ { 8 };
};

#endif
