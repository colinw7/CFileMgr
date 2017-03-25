#include <CQPixelRendererSubCanvas.h>
#include <CQPixelRendererScrolledCanvas.h>
#include <CQPixelRendererCanvas.h>

#include <QGridLayout>
#include <QScrollBar>

CQPixelRendererScrolledCanvas::
CQPixelRendererScrolledCanvas(QWidget *parent) :
 QWidget(parent), hstep_(1), vstep_(1)
{
  QGridLayout *grid = new QGridLayout(this);
  grid->setMargin(2); grid->setSpacing(2);

  canvas_ = new CQPixelRendererSubCanvas(this, this);

  hscroll_ = new QScrollBar(Qt::Horizontal, this);
  vscroll_ = new QScrollBar(Qt::Vertical  , this);

  QObject::connect(hscroll_, SIGNAL(valueChanged(int)), canvas_, SLOT(scrolledSlot()));
  QObject::connect(vscroll_, SIGNAL(valueChanged(int)), canvas_, SLOT(scrolledSlot()));

  grid->addWidget(canvas_ , 0, 0);
  grid->addWidget(hscroll_, 1, 0);
  grid->addWidget(vscroll_, 0, 1);

  updateScrollbars();

  setFocusProxy(canvas_);

  canvas_->setFocusPolicy(Qt::StrongFocus);
}

CQPixelRendererScrolledCanvas::
~CQPixelRendererScrolledCanvas()
{
  delete canvas_;
}

CQPixelRendererCanvas *
CQPixelRendererScrolledCanvas::
getCanvas() const
{
  return canvas_;
}

QScrollBar *
CQPixelRendererScrolledCanvas::
getHScroll() const
{
  return hscroll_;
}

QScrollBar *
CQPixelRendererScrolledCanvas::
getVScroll() const
{
  return vscroll_;
}

const CIPoint2D &
CQPixelRendererScrolledCanvas::
getMousePos()
{
  return canvas_->getMousePos();
}

void
CQPixelRendererScrolledCanvas::
allowSelection()
{
  canvas_->allowSelection();
}

void
CQPixelRendererScrolledCanvas::
setContentsChanged(bool flag)
{
  canvas_->setContentsChanged(flag);
}

void
CQPixelRendererScrolledCanvas::
signalChanged()
{
  canvas_->signalChanged();
}

const CIBBox2D &
CQPixelRendererScrolledCanvas::
getSelectionRect()
{
  return canvas_->getSelectionRect();
}

void
CQPixelRendererScrolledCanvas::
setSelectionRect(const CIBBox2D &rect)
{
  return canvas_->setSelectionRect(rect);
}

void
CQPixelRendererScrolledCanvas::
resetSelectionRect()
{
  return canvas_->resetSelectionRect();
}

void
CQPixelRendererScrolledCanvas::
setVSize(const CISize2D &size)
{
  canvas_->setVSize(size);

  updateScrollbars();
}

const CISize2D &
CQPixelRendererScrolledCanvas::
getVSize() const
{
  return canvas_->getVSize();
}

void
CQPixelRendererScrolledCanvas::
showHBar(bool flag)
{
  flag ? hscroll_->show() : hscroll_->hide();
}

void
CQPixelRendererScrolledCanvas::
showVBar(bool flag)
{
  flag ? vscroll_->show() : vscroll_->hide();
}

void
CQPixelRendererScrolledCanvas::
clearSelection()
{
  return canvas_->clearSelection();
}

void
CQPixelRendererScrolledCanvas::
scrollTo(const CIPoint2D &point, CScrollType pos)
{
  bool changed = false;

  int xmin = hscroll_->value();
  int xmax = xmin + canvas_->width () - 1;

  int ymin = vscroll_->value();
  int ymax = ymin + canvas_->height() - 1;

  int d;

  if      ((d = xmin - point.getX()) > 0) {
    int x = xmin - d;

    if (hscroll_->value() != x) {
      hscroll_->setValue(x);

      changed = true;
    }
  }
  else if ((d = point.getX() - xmax) > 0) {
    int x = xmin + d;

    if (hscroll_->value() != x) {
      hscroll_->setValue(x);

      changed = true;
    }
  }

  int y = ymin;

  if      (pos == CSCROLL_TYPE_VISIBLE) {
    if      ((d = ymin - point.getY()) > 0)
      y = ymin - d;
    else if ((d = point.getY() - ymax) > 0)
      y = ymin + d;
  }
  else if (pos ==  CSCROLL_TYPE_TOP)
    y = point.getY();
  else if (pos ==  CSCROLL_TYPE_MIDDLE)
    y = point.getY() - canvas_->height()/2;
  else if (pos ==  CSCROLL_TYPE_BOTTOM)
    y = point.getY() - canvas_->height() - 1;

  int miny = vscroll_->minimum();
  int maxy = vscroll_->maximum();

  y = std::min(std::max(y, miny), maxy);

  if (vscroll_->value() != y) {
    vscroll_->setValue(y);

    changed = true;
  }

  if (changed)
    updateOffset();
}

void
CQPixelRendererScrolledCanvas::
scrollTo(const CIBBox2D &rect, CScrollType pos)
{
  bool changed = false;

  int xmin = hscroll_->value();
  int xmax = xmin + canvas_->width () - 1;

  int ymin = vscroll_->value();
  int ymax = ymin + canvas_->height() - 1;

  int d;

  if      ((d = xmin - rect.getXMin()) > 0) {
    int x = xmin - d;

    if (hscroll_->value() != x) {
      hscroll_->setValue(x);

      changed = true;
    }
  }
  else if ((d = rect.getXMax() - xmax) > 0) {
    int x = xmin + d;

    if (hscroll_->value() != x) {
      hscroll_->setValue(x);

      changed = true;
    }
  }

  int y = ymin;

  if      (pos == CSCROLL_TYPE_VISIBLE) {
    if      ((d = ymin - rect.getYMin()) > 0)
      y = ymin - d;
    else if ((d = rect.getYMax() - ymax) > 0)
      y = ymin + d;
  }
  else if (pos == CSCROLL_TYPE_TOP)
    y = rect.getYMin();
  else if (pos == CSCROLL_TYPE_MIDDLE)
    y = rect.getYMin() - canvas_->height()/2;
  else if (pos == CSCROLL_TYPE_BOTTOM)
    y = rect.getYMin() - canvas_->height() - 1;

  int miny = vscroll_->minimum();
  int maxy = vscroll_->maximum();

  y = std::min(std::max(y, miny), maxy);

  if (vscroll_->value() != y) {
    vscroll_->setValue(y);

    changed = true;
  }

  if (changed)
    updateOffset();
}

void
CQPixelRendererScrolledCanvas::
scroll(int delta)
{
  vscroll_->setValue(vscroll_->value() + delta*vstep_);
}

void
CQPixelRendererScrolledCanvas::
scrolledSlot()
{
  updateOffset();

  canvas_->scrolled();

  scrolled();
}

void
CQPixelRendererScrolledCanvas::
applyOffset(CIPoint2D &point)
{
  canvas_->applyOffset(point);
}

void
CQPixelRendererScrolledCanvas::
unapplyOffset(CIPoint2D &point)
{
  canvas_->unapplyOffset(point);
}

void
CQPixelRendererScrolledCanvas::
trackMouse(bool flag)
{
  canvas_->trackMouse(flag);
}

CQWidgetPixelRenderer *
CQPixelRendererScrolledCanvas::
getRenderer()
{
  return canvas_->getRenderer();
}

void
CQPixelRendererScrolledCanvas::
updateOffset()
{
  CIPoint2D offset(-hscroll_->value(), -vscroll_->value());

  canvas_->setOffset(offset);
}

const CIPoint2D &
CQPixelRendererScrolledCanvas::
getOffset() const
{
  return canvas_->getOffset();
}

void
CQPixelRendererScrolledCanvas::
updateScrollbars()
{
  int hstep = canvas_->width ();
  int vstep = canvas_->height();

  hscroll_->setSingleStep(hstep_); hscroll_->setPageStep(hstep);
  vscroll_->setSingleStep(vstep_); vscroll_->setPageStep(vstep);

  const CISize2D &vsize = canvas_->getVSize();

  int mx = std::max(vsize.width  - 1 - hstep, 0);
  int my = std::max(vsize.height - 1 - vstep, 0);

  hscroll_->setMinimum(0); hscroll_->setMaximum(mx);
  vscroll_->setMinimum(0); vscroll_->setMaximum(my);

  const CIPoint2D &offset = canvas_->getOffset();

  hscroll_->setValue(-offset.getX());
  vscroll_->setValue(-offset.getY());
}
