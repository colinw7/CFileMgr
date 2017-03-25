#include <CQImageCanvas.h>
#include <CQWinWidget.h>
#include <CQPaintTools.h>
#include <CQZoomCursor.h>
#include <CQUtil.h>

#include <QRubberBand>
#include <QTimer>
#include <QVBoxLayout>
#include <QPainter>
#include <QMouseEvent>

CQImageCanvas::
CQImageCanvas(QWidget *parent) :
 QWidget(parent)
{
  setObjectName("imageCanvas");

  bg_ = CRGBA(0,0,0);

  pen_.fg = CRGBA(1,0,0);
  pen_.bg = CRGBA(0,1,0);

  CImageNoSrc src;

  image_ = CImageMgrInst->createImage(src);

  image_->setDataSize(1, 1);

  vimage_ = image_->dup();

  zoom_cursor_ = new CQZoomCursor(image_);
}

CQImageCanvas::
~CQImageCanvas()
{
}

void
CQImageCanvas::
addTools()
{
  CQPaintToolsWindow *window = new CQPaintToolsWindow(this);

  connect(window->getTools(), SIGNAL(select ()), this, SLOT(doSelect ()));
  connect(window->getTools(), SIGNAL(pen    ()), this, SLOT(doPen    ()));
  connect(window->getTools(), SIGNAL(dropper()), this, SLOT(doDropper()));
  connect(window->getTools(), SIGNAL(fill   ()), this, SLOT(doFill   ()));
}

void
CQImageCanvas::
setMode(Mode mode)
{
  mode_ = mode;
}

void
CQImageCanvas::
setBackground(const CRGBA &rgba)
{
  bg_ = rgba;
}

void
CQImageCanvas::
setPenBackground(const CRGBA &rgba)
{
  pen_.bg = rgba;
}

void
CQImageCanvas::
setPenForeground(const CRGBA &rgba)
{
  pen_.fg = rgba;
}

void
CQImageCanvas::
setOffset(int, int)
{
}

void
CQImageCanvas::
resizeEvent(QResizeEvent *)
{
  if (timer_ == NULL) {
    timer_ = new QTimer(this);

    timer_->setSingleShot(true);

    connect(timer_, SIGNAL(timeout()), this, SLOT(resizeTimeout()));
  }

  timer_->start(50);
}

void
CQImageCanvas::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  painter.fillRect(rect(), QBrush(CQUtil::rgbaToColor(bg_)));

  if (! vimage_.isValid())
    return;

  double s = getZoomFactor();

  // if zoom factor < 1 then vimage is scaled image ...
  if (s <= 1) {
    int dx = 0; //max((w - int(vimage_->getWidth ()))/2, 0);
    int dy = 0; //max((h - int(vimage_->getHeight()))/2, 0);

    painter.drawImage(QPoint(dx, dy), CQUtil::toQImage(vimage_));
  }
  // ... otherwise draw scaled image
  else {
    int iwidth  = width ();
    int iheight = height();

    int iwidth1  = int(s*vimage_->getWidth ());
    int iheight1 = int(s*vimage_->getHeight());

    double is = 1.0/s;

    int x1 = 0;
    int x2 = std::min(iwidth1  - 1, iwidth  - 1);
    int y1 = 0;
    int y2 = std::min(iheight1 - 1, iheight - 1);

    CRGBA rgba;

    for (int y = y1; y <= y2; ++y) {
      for (int x = x1; x <= x2; ++x) {
        int ix = (x - x1)*is;
        int iy = (y - y1)*is;

        vimage_->getRGBAPixel(ix, iy, rgba);

        painter.setPen(CQUtil::rgbaToColor(rgba));

        painter.drawPoint(QPoint(x, y));
      }
    }
  }

  if (getZoomCursor()) {
    CIPoint2D pos = CQUtil::fromQPoint(mapFromGlobal(QCursor::pos()));

    CImagePtr image = zoom_cursor_->getImage(pos);

    painter.drawImage(CQUtil::toQPoint(pos), CQUtil::toQImage(image));
  }
}

void
CQImageCanvas::
updateImage(const std::string &filename)
{
  CImageFileSrc src(filename);

  CImagePtr image = CImageMgrInst->createImage(src);

  updateImage(image);
}

void
CQImageCanvas::
updateImage(CImagePtr image)
{
  image_ = image;

  if (image_->hasColormap())
    image_->convertToRGB();

  updateVImage(true);

  //updateScrollbars();
}

void
CQImageCanvas::
resizeTimeout()
{
  updateVImage(false);
}

void
CQImageCanvas::
updateVImage(bool image_changed)
{
  // if scale to fill current window size
  if (getFillScreen()) {
    int width1  = width();
    int height1 = height();

    // resize to match window (with aspect preserve if active)
    if (getKeepAspect())
      vimage_ = image_->resizeKeepAspect(width1, height1);
    else
      vimage_ = image_->resize(width1, height1);

    zoom_factor_.setValue((1.0*vimage_->getWidth())/image_->getWidth());
  }
  else {
    // if image or zoom factor changed then update zoomed image
    if (image_changed || zoom_factor_.valueChanged()) {
      double s = getZoomFactor();

      if (s < 1)
        vimage_ = image_->scaleKeepAspect(s);
      else
        vimage_ = image_->dup();
    }
  }

  if (image_changed)
    zoom_cursor_->setImage(image_);
  else
    zoom_cursor_->setFactor(getZoomFactor());

  //setVSize(getZoomFactor()*image_->getSize());

  update();
}

int
CQImageCanvas::
getImageWidth() const
{
  if (vimage_.isValid())
    return vimage_->getWidth();

  return 0;
}

int
CQImageCanvas::
getImageHeight() const
{
  if (vimage_.isValid())
    return vimage_->getHeight();

  return 0;
}

void
CQImageCanvas::
clear()
{
  if (! image_.isValid())
    return;

  vimage_->setRGBAData(pen_.bg);

  updateVImage(true);
}

void
CQImageCanvas::
invert()
{
  if (! image_.isValid())
    return;

  //image_->setWindow(getSelectionRect());

  image_->invert();

  image_->resetWindow();

  updateVImage(true);
}

void
CQImageCanvas::
grayscale()
{
  if (! image_.isValid())
    return;

  //image_->setWindow(getSelectionRect());

  image_->grayScale();

  image_->resetWindow();

  updateVImage(true);
}

void
CQImageCanvas::
sepia()
{
  if (! image_.isValid())
    return;

  //image_->setWindow(getSelectionRect());

  image_->sepia();

  image_->resetWindow();

  updateVImage(true);
}

void
CQImageCanvas::
gaussianBlur(double xBlur, double yBlur, int xSize, int ySize)
{
  if (! image_.isValid())
    return;

  //image_->setWindow(getSelectionRect());

  image_->gaussianBlur(xBlur, yBlur, xSize, ySize);

  image_->resetWindow();

  updateVImage(true);
}

void
CQImageCanvas::
turbulence(bool fractal, double freq, int octaves, int seed)
{
  if (! image_.isValid())
    return;

  //image_->setWindow(getSelectionRect());

  image_->turbulence(fractal, freq, octaves, seed);

  image_->resetWindow();

  updateVImage(true);
}

void
CQImageCanvas::
doResize(int width, int height, bool scale, bool aspect)
{
  if (! image_.isValid())
    return;

  if (scale) {
    if (aspect)
      image_->reshapeKeepAspect(width, height);
    else
      image_->reshape(width, height);
  }
  else
    image_->setDataSize(width, height);

  updateVImage(true);
}

void
CQImageCanvas::
setFillScreen(bool fill_screen)
{
  fill_screen_ = fill_screen;

  updateVImage(false);
}

void
CQImageCanvas::
setZoomCursor(bool zoom_cursor_active)
{
  zoom_cursor_active_ = zoom_cursor_active;

  if (zoom_cursor_active_) {
    setCursor(Qt::BlankCursor);

    setMouseTracking(true);
  }
  else
    setCursor(Qt::ArrowCursor);
}

void
CQImageCanvas::
setZoomFactor(double factor)
{
  zoom_factor_.setValue(factor);

  updateVImage(false);
}

double
CQImageCanvas::
getZoomCursorFactor() const
{
  return zoom_cursor_->getFactor();
}

void
CQImageCanvas::
setZoomCursorFactor(double f)
{
  zoom_cursor_->setFactor(f);

  update();
}

int
CQImageCanvas::
getZoomCursorWidth() const
{
  return zoom_cursor_->getWidth();
}

void
CQImageCanvas::
setZoomCursorWidth(int w)
{
  zoom_cursor_->setWidth(w);

  update();
}

void
CQImageCanvas::
setKeepAspect(bool keep_aspect)
{
  keep_aspect_ = keep_aspect;

  updateVImage(false);
}

void
CQImageCanvas::
selectAll()
{
  //int w = vimage_->getWidth();
  //int h = vimage_->getHeight();

  //setSelectionRect(CIBBox2D(0, 0, w - 1, h - 1));
}

void
CQImageCanvas::
selectNone()
{
  //resetSelectionRect();
}

void
CQImageCanvas::
zoomIncrease()
{
  setZoomFactor(2.0*getZoomFactor());
}

void
CQImageCanvas::
zoomDecrease()
{
  setZoomFactor(0.5*getZoomFactor());
}

void
CQImageCanvas::
mousePressEvent(QMouseEvent *event)
{
  press_pos_ = CQUtil::fromQPoint(event->pos());

  if      (mode_ == Mode::SELECT) {
    if (event->button() != Qt::LeftButton) return;
  }
  else if (mode_ == Mode::PEN) {
    if (event->button() != Qt::LeftButton) return;

    vimage_->setRGBAPixel(press_pos_, pen_.fg);

    update();
  }
  else if (mode_ == Mode::DROPPER) {
    if (event->button() != Qt::LeftButton && event->button() != Qt::MidButton) return;

    if (vimage_->validPixel(press_pos_)) {
      if (event->button() == Qt::LeftButton) {
        vimage_->getRGBAPixel(press_pos_, pen_.fg);

        emit penFgChanged();
      }
      else {
        vimage_->getRGBAPixel(press_pos_, pen_.bg);

        emit penBgChanged();
      }
    }
  }

  pressed_ = true;
}

void
CQImageCanvas::
mouseMoveEvent(QMouseEvent *)
{
  if (pressed_) {
    if      (mode_ == Mode::SELECT) {
      //pointerMotion(event);
    }
    else if (mode_ == Mode::PEN) {
      //vrenderer_->setForeground(pen_.fg);

      //vrenderer_->drawLine(press_pos_, event->getPosition());

      //press_pos_ = event->getPosition();

      //update();
    }
  }
  else {
    //QString qstr = QString("( %1, %2 )").arg(x).arg(y);

    //paint_->setStatusMessage(qstr);

    //update();
  }

  if (getZoomCursor())
    update();
}

void
CQImageCanvas::
mouseReleaseEvent(QMouseEvent *event)
{
  if (! pressed_) return;

  if      (mode_ == Mode::SELECT) {
    if (event->button() != Qt::LeftButton) return;
  }
}

void
CQImageCanvas::
keyPressEvent(QKeyEvent *e)
{
  CKeyEvent *event = CQUtil::convertEvent(e);

  emit keyPressed(*event);

  if      (event->isType(CKEY_TYPE_Plus))
    setZoomFactor(2.0*getZoomFactor());
  else if (event->isType(CKEY_TYPE_Minus))
    setZoomFactor(0.5*getZoomFactor());
}

void
CQImageCanvas::
keyReleaseEvent(QKeyEvent *)
{
}

QSize
CQImageCanvas::
sizeHint() const
{
  return QSize(std::max(image_->getWidth(), 256U), std::max(image_->getHeight(), 256U));
}
