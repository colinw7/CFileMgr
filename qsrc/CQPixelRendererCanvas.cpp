#include <CQPixelRendererCanvas.h>
#include <CQWidgetPixelRenderer.h>
#include <CQUtil.h>

#include <QWidget>
#include <QMouseEvent>
#include <QRubberBand>

CQPixelRendererCanvas::
CQPixelRendererCanvas(QWidget *parent) :
 QWidget(parent)
{
  renderer_ = new CQWidgetPixelRenderer(this);

  setFocusPolicy(Qt::StrongFocus);

  setMouseTracking(true);
}

CQPixelRendererCanvas::
~CQPixelRendererCanvas()
{
  delete renderer_;
  delete rubber_band_;
}

void
CQPixelRendererCanvas::
allowSelection(bool allow_selection, bool show_rubber_band)
{
  trackMouse(allow_selection);

  allow_selection_  = allow_selection;
  show_rubber_band_ = show_rubber_band;
}

void
CQPixelRendererCanvas::
setContentsChanged(bool flag)
{
  renderer_->setContentsChanged(flag);
}

void
CQPixelRendererCanvas::
signalChanged()
{
  setContentsChanged(true);

  update();
}

void
CQPixelRendererCanvas::
setSelectionRect(const CIBBox2D &rect)
{
  selection_ = rect;

  selectionNotify(CBUTTON_ACTION_RELEASE);
}

void
CQPixelRendererCanvas::
resetSelectionRect()
{
  selection_.reset();

  selectionNotify(CBUTTON_ACTION_RELEASE);
}

void
CQPixelRendererCanvas::
setOffset(const CIPoint2D &offset)
{
  renderer_->setOffset(offset);

  if (show_rubber_band_)
    updateRubberband();
}

const CIPoint2D &
CQPixelRendererCanvas::
getOffset() const
{
  return renderer_->getOffset();
}

void
CQPixelRendererCanvas::
setVSize(const CISize2D &size)
{
  renderer_->setVSize(size);
}

const CISize2D &
CQPixelRendererCanvas::
getVSize() const
{
  return renderer_->getVSize();
}

void
CQPixelRendererCanvas::
trackMouse(bool flag)
{
  setMouseTracking(flag);
}

void
CQPixelRendererCanvas::
paintEvent(QPaintEvent *)
{
  if (renderer_->getContentsChanged()) {
    renderer_->startDoubleBuffer(clear_);

    draw();

    renderer_->endDoubleBuffer();

    setContentsChanged(false);
  }
  else
    renderer_->copyDoubleBuffer();
}

void
CQPixelRendererCanvas::
resizeEvent(QResizeEvent *)
{
  setContentsChanged(true);

  resize();
}

void
CQPixelRendererCanvas::
mousePressEvent(QMouseEvent *event)
{
  if (pressed_) return;

  pressed_ = true;

  CMouseEvent *event1 = CQUtil::convertEvent(event);

  press_button_ = event1->getButton();
  press_pos_    = event1->getPosition();

  unapplyOffset(press_pos_);

  event1->setPosition(press_pos_);

  if (allow_selection_) {
    if (event1->getButton() == CBUTTON_LEFT) {
      selection_.set(press_pos_, press_pos_);

      if (show_rubber_band_)
        updateRubberband();

      selectionNotify(CBUTTON_ACTION_PRESS);
    }
  }

  mousePress(*event1);
}

void
CQPixelRendererCanvas::
mouseMoveEvent(QMouseEvent *event)
{
  CMouseEvent *event1 = CQUtil::convertEvent(event);

  CIPoint2D motion_pos = event1->getPosition();

  unapplyOffset(motion_pos);

  event1->setPosition(motion_pos);

  if (pressed_) {
    event1->setButton(press_button_);

    if (allow_selection_) {
      if (event1->getButton() == CBUTTON_LEFT) {
        selection_.set(press_pos_, motion_pos);

        if (show_rubber_band_)
          updateRubberband();

        selectionNotify(CBUTTON_ACTION_MOTION);
      }
    }

    mouseMotion(*event1);
  }
  else {
    if (motion_pos != mouse_pos_)
      pointerMotion(*event1);

    mouse_pos_ = motion_pos;
  }
}

void
CQPixelRendererCanvas::
mouseReleaseEvent(QMouseEvent *event)
{
  if (! pressed_) return;

  CMouseEvent *event1 = CQUtil::convertEvent(event);

  CIPoint2D release_pos = event1->getPosition();

  unapplyOffset(release_pos);

  event1->setPosition(release_pos);

  if (allow_selection_) {
    if (event1->getButton() == CBUTTON_LEFT) {
      selection_.set(press_pos_, release_pos);

      if (show_rubber_band_)
        updateRubberband();

      selectionNotify(CBUTTON_ACTION_RELEASE);
    }
  }

  mouseRelease(*event1);

  pressed_ = false;
}

void
CQPixelRendererCanvas::
mouseDoubleClickEvent(QMouseEvent *event)
{
  CMouseEvent *event1 = CQUtil::convertEvent(event);

  press_pos_ = event1->getPosition();

  unapplyOffset(press_pos_);

  event1->setPosition(press_pos_);

  event1->setCount(2);

  mouseRelease(*event1);
}

void
CQPixelRendererCanvas::
wheelEvent(QWheelEvent *event)
{
  scroll(-event->delta()/120);
}

void
CQPixelRendererCanvas::
keyPressEvent(QKeyEvent *event)
{
  CKeyEvent *event1 = CQUtil::convertEvent(event);

  keyPress(*event1);
}

void
CQPixelRendererCanvas::
keyReleaseEvent(QKeyEvent *event)
{
  CKeyEvent *event1 = CQUtil::convertEvent(event);

  keyRelease(*event1);
}

void
CQPixelRendererCanvas::
scrolled()
{
  setContentsChanged(true);

  if (show_rubber_band_)
    updateRubberband();

  repaint();
}

void
CQPixelRendererCanvas::
clearSelection()
{
  selection_.reset();

  if (show_rubber_band_)
    updateRubberband();
}

void
CQPixelRendererCanvas::
updateRubberband()
{
  if (selection_.isSet() && selection_.getMaxDim() > 2) {
    if (rubber_band_ == 0)
      rubber_band_ = new QRubberBand(QRubberBand::Rectangle, this);

    int x = selection_.getXMin (); int y = selection_.getYMin  ();
    int w = selection_.getWidth(); int h = selection_.getHeight();

    CIPoint2D point(x, y);

    applyOffset(point);

    rubber_band_->setGeometry(point.x, point.y, w + 1, h + 1);

    rubber_band_->show();
  }
  else {
    if (rubber_band_ != 0)
      rubber_band_->hide();
  }
}

void
CQPixelRendererCanvas::
applyOffset(CIPoint2D &point)
{
  renderer_->applyOffset(point);
}

void
CQPixelRendererCanvas::
unapplyOffset(CIPoint2D &point)
{
  renderer_->unapplyOffset(point);
}
