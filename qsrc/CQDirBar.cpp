#include <CQDirBar.h>
#include <CQDirView.h>
#include <CQFileMgr.h>
#include <CQStyleMgr.h>
#include <CQUtil.h>
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

  //auto *layout = CQUtil::makeLayout<QHBoxLayout>(this, 0, 0);

  //auto *llayout = CQUtil::makeLayout<QHBoxLayout>(0, 0);
  //auto *rlayout = CQUtil::makeLayout<QHBoxLayout>(0, 0);

  //rlayout->addStretch();

  //layout->addLayout(llayout);
  //layout->addLayout(rlayout);

  //layout_ = llayout;

  updateDir();

  fontChanged();

  connect(CQDirViewInst, SIGNAL(dirChangedSignal(const QString &)),
          this, SLOT(updateDir()));

  connect(CQStyleMgrInst, SIGNAL(fontChanged()), this, SLOT(fontChanged()));
}

CQDirBar::
~CQDirBar()
{
}

void
CQDirBar::
setBorder(int b)
{
  border_ = b;

  for (auto *widget : widgets_)
    widget->setBorder(b);

  updateSizes();
}

void
CQDirBar::
setArrowWidth(int w)
{
  arrowWidth_ = w;

  for (auto *widget : widgets_)
    widget->setArrowWidth(w);

  updateSizes();
}

void
CQDirBar::
updateSizes()
{
  for (auto *widget : widgets_) {
    auto ws = widget->sizeHint();

    widget->setFixedSize(ws);
  }
}

void
CQDirBar::
fontChanged()
{
  QFontMetrics fm(font());

  setArrowWidth(fm.height()/3);
}

void
CQDirBar::
updateDir()
{
  const auto &dirname = CDirViewInst->getDirName();

  //------

  std::vector<std::string> fields;

  if (dirname != "/")
    CStrUtil::addFields(dirname, fields, "/");
  else
    fields.push_back("");

  auto num_fields = fields.size();

  setNumWidgets(num_fields);

  std::string str;

  for (uint i = 0; i < num_fields; ++i) {
    const auto &field = fields[i];

    auto *widget = widgets_[i];

    if (i == 0) {
      widget->setText("/");
      widget->setDir ("/");
    }
    else {
      str += "/" + field;

      widget->setText(field.c_str());
      widget->setDir (str.c_str());
    }

    widget->setFirst(i == 0);
    widget->setLast (i == num_fields - 1);
  }

  updateSizes();
}

void
CQDirBar::
setNumWidgets(uint num_widgets)
{
  if (num_widgets_ < num_widgets) {
    for (uint i = num_widgets_; i < num_widgets; ++i) {
      auto *item = new CQDirBarItem(this, " ");

      //layout_->addWidget(item);

      widgets_.push_back(item);
    }
  }
  else if (num_widgets_ > num_widgets) {
    for (uint i = num_widgets; i < num_widgets_; ++i) {
      auto *item = widgets_.back();

      widgets_.pop_back();

      delete item;
    }
  }

  num_widgets_ = num_widgets;
}

void
CQDirBar::
resizeEvent(QResizeEvent *)
{
  QFontMetrics fm(font());

  int x = 0;

  clipped_      = false;
  clippedWidth_ = 0;

  for (size_t i = 0; i < num_widgets_; ++i) {
    int i1 = num_widgets_ - 1 - i;

    auto *widget = widgets_[i1];

    if (x + widget->width() > width()) {
      clippedWidth_ = x;
      clipped_ = true;
      break;
    }

    x += widget->width();
  }

  int xc = fm.horizontalAdvance("...") + border();

  if (clipped_)
    x = clippedWidth_ + xc;
  else
    x = 0;

  for (size_t i = 0; i < num_widgets_; ++i) {
    int i1 = num_widgets_ - 1 - i;

    auto *widget = (clipped_ ? widgets_[i1] : widgets_[i]);

    if (clipped_) {
      int x1 = x - widget->width();

      widget->move(x1, 0);

      widget->setVisible(x1 >= xc);
      widget->setClipped(x1 < xc);

      x = x1;
    }
    else {
      widget->move(x, 0);

      widget->setVisible(true);
      widget->setClipped(false);

      x += widget->width();
    }
  }
}

void
CQDirBar::
paintEvent(QPaintEvent *)
{
  if (clipped_) {
    QPainter painter(this);

    auto rect = this->rect();

    rect.setWidth(clippedWidth_);

    painter.drawText(rect, "...");
  }
}

QSize
CQDirBar::
sizeHint() const
{
  int w = 2*border();
  int h = 2*border();

  for (uint i = 0; i < num_widgets_; ++i) {
    auto *widget = widgets_[i];

    auto s = widget->sizeHint();

    w += s.width();
    h = std::max(h, s.height());
  }

  return QSize(w, h);
}

QSize
CQDirBar::
minimumSizeHint() const
{
  int w = 2*border();
  int h = 2*border();

  for (uint i = 0; i < num_widgets_; ++i) {
    auto *widget = widgets_[i];

    auto s = widget->minimumSizeHint();

    w += s.width();
    h = std::max(h, s.height());
  }

  return QSize(w, h);
}

//--------

CQDirBarItem::
CQDirBarItem(CQDirBar *bar, const QString &text) :
 QWidget(bar), bar_(bar), text_(text)
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
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  painter.setFont(bar_->font());

//auto border     = this->border();
  auto arrowWidth = this->arrowWidth();

  auto rect = this->rect();

  auto bg = palette().window().color();

  painter.fillRect(rect, bg);

  //---

  // draw text
  auto rect1 = rect.adjusted(0, 0, -arrowWidth, 0);

//painter.fillRect(rect1, Qt::red);

  if (! isClipped())
    painter.drawText(rect1, text());
  else
    painter.drawText(rect1, "...");

  //---

  // draw border
  int x1 = rect.left (); int y1 = rect.top   ();
  int x3 = rect.right(); int y3 = rect.bottom();

  int x2 = x3 - arrowWidth;
  int y2 = (y1 + y3)/2;

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
  QFontMetrics fm(font());

  auto border     = this->border();
  auto arrowWidth = this->arrowWidth();

  int w = fm.horizontalAdvance(text()) + 2*border + arrowWidth;
  int h = fm.height() + 2*border;

  return QSize(w, h);
}

QSize
CQDirBarItem::
minimumSizeHint() const
{
  QFontMetrics fm(font());

  auto border = this->border();

  int h = fm.height() + 2*border;

  return QSize(1, h);
}
