#include <CQDirBar.h>
#include <CQDirView.h>
#include <CQFileMgr.h>
#include <CDir.h>
#include <CStrUtil.h>

#include <QHBoxLayout>
#include <QPainter>
#include <QPaintEvent>

CQDirBar::
CQDirBar(QWidget *parent) :
 QWidget(parent)
{
  setObjectName("dirBar");

  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  QHBoxLayout *llayout = new QHBoxLayout;
  llayout->setMargin(0); llayout->setSpacing(0);

  QHBoxLayout *rlayout = new QHBoxLayout;
  rlayout->setMargin(0); rlayout->setSpacing(0);

  rlayout->addStretch();

  layout->addLayout(llayout);
  layout->addLayout(rlayout);

  layout_ = llayout;

  updateDir();

  connect(CQDirViewInst, SIGNAL(dirChangedSignal(const QString &)),
          this, SLOT(updateDir()));
}

CQDirBar::
~CQDirBar()
{
}

void
CQDirBar::
updateDir()
{
  const std::string &dirname = CDirViewInst->getDirName();

  //------

  std::vector<std::string> fields;

  if (dirname != "/")
    CStrUtil::addFields(dirname, fields, "/");
  else
    fields.push_back("");

  uint num_fields = fields.size();

  setNumWidgets(num_fields);

  std::string str = "";

  for (uint i = 0; i < num_fields; ++i) {
    if (i == 0) {
      widgets_[i]->setText("/");
      widgets_[i]->setDir ("/");
    }
    else {
      str += "/" + fields[i];

      widgets_[i]->setText(fields[i].c_str());
      widgets_[i]->setDir (str.c_str());
    }

    QSize ws = widgets_[i]->minimumSizeHint();

    widgets_[i]->setFixedSize(ws);

    widgets_[i]->setFirst(i == 0);
    widgets_[i]->setLast (i == num_fields - 1);
  }
}

void
CQDirBar::
setNumWidgets(uint num_widgets)
{
  if (num_widgets_ < num_widgets) {
    for (uint i = num_widgets_; i < num_widgets; ++i) {
      CQDirBarItem *item = new CQDirBarItem(this, " ");

      layout_->addWidget(item);

      widgets_.push_back(item);
    }
  }
  else if (num_widgets_ > num_widgets) {
    for (uint i = num_widgets; i < num_widgets_; ++i) {
      CQDirBarItem *item = widgets_.back();

      widgets_.pop_back();

      delete item;
    }
  }

  num_widgets_ = num_widgets;
}

QSize
CQDirBar::
sizeHint() const
{
  return minimumSizeHint();
}

QSize
CQDirBar::
minimumSizeHint() const
{
  int w = 4;
  int h = 4;

  for (uint i = 0; i < num_widgets_; ++i) {
    QSize s = widgets_[i]->minimumSizeHint();

    w += s.width();
    h = std::max(h, s.height());
  }

  return QSize(w, h);
}

//--------

CQDirBarItem::
CQDirBarItem(CQDirBar *bar, const QString &text) :
 QWidget(0), bar_(bar), text_(text)
{
  setObjectName("item");
}

CQDirBarItem::
~CQDirBarItem()
{
}

void
CQDirBarItem::
setText(const QString &text)
{
  text_ = text;

  update();
}

void
CQDirBarItem::
setDir(const QString &dir)
{
  dir_ = dir;

  update();
}

void
CQDirBarItem::
mousePressEvent(QMouseEvent *)
{
  CDirViewInst->setDirName(dir_.toStdString());
}

void
CQDirBarItem::
paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  QRect rect  = event->rect();

  QRect rect1 = rect.adjusted(border_, 0, -border_ - arrowWidth_, 0);

  painter.drawText(rect1, text_);

  int x1 = rect.left();
  int x2 = rect.right() - arrowWidth_;
  int x3 = rect.right();
  int y1 = rect.top();
  int y2 = (rect.top() + rect.bottom())/2;
  int y3 = rect.bottom();

  QColor bg = palette().window().color();

  painter.setPen(bg.darker(110));

  painter.drawLine(x2 - 1, y1, x3 - 1, y2);
  painter.drawLine(x3 - 1, y2, x2 - 1, y3);

  if (! last_) {
    painter.drawLine(x1, y1 + 1, x3, y1 + 1);
    painter.drawLine(x1, y3 - 1, x3, y3 - 1);
  }
  else {
    painter.drawLine(x1, y1 + 1, x2 - 1, y1 + 1);
    painter.drawLine(x1, y3 - 1, x2 - 1, y3 - 1);
  }

  painter.setPen(bg.darker(140));

  painter.drawLine(x2, y1, x3, y2);
  painter.drawLine(x3, y2, x2, y3);

  if (! last_) {
    painter.drawLine(x1, y1, x3, y1);
    painter.drawLine(x1, y3, x3, y3);
  }
  else {
    painter.drawLine(x1, y1, x2, y1);
    painter.drawLine(x1, y3, x2, y3);
  }
}

QSize
CQDirBarItem::
sizeHint() const
{
  return minimumSizeHint();
}

QSize
CQDirBarItem::
minimumSizeHint() const
{
  QFontMetrics fm(font());

  int w = fm.horizontalAdvance(text_) + 2*border_ + arrowWidth_;
  int h = fm.height() + 4;

  return QSize(w, h);
}
