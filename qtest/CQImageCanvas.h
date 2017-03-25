#ifndef CQIMAGE_CANVAS_H
#define CQIMAGE_CANVAS_H

#include <QWidget>

#include <CIBBox2D.h>
#include <CImage.h>
#include <CImagePtr.h>
#include <CValueHistory.h>
#include <CEvent.h>
#include <CAutoPtr.h>

class CQWinWidget;
class CQZoomCursor;
class QTimer;

class CQImageCanvas : public QWidget {
  Q_OBJECT

  Q_PROPERTY(double zoomFactor       READ getZoomFactor       WRITE setZoomFactor)
  Q_PROPERTY(bool   fillScreen       READ getFillScreen       WRITE setFillScreen)
  Q_PROPERTY(bool   keepAspect       READ getKeepAspect       WRITE setKeepAspect)
  Q_PROPERTY(bool   zoomCursor       READ getZoomCursor       WRITE setZoomCursor)
  Q_PROPERTY(double zoomCursorFactor READ getZoomCursorFactor WRITE setZoomCursorFactor)
  Q_PROPERTY(int    zoomCursorSize   READ getZoomCursorWidth  WRITE setZoomCursorWidth)

 public:
  enum class Mode {
    NONE,
    SELECT,
    PEN,
    DROPPER,
    FILL
  };

 public:
  CQImageCanvas(QWidget *parent=0);

  virtual ~CQImageCanvas();

  void addTools();

  void setMode(Mode mode);

  void setBackground(const CRGBA &rgba);

  void setPenBackground(const CRGBA &rgba);
  const CRGBA &getPenBackground() const { return pen_.bg; }

  void setPenForeground(const CRGBA &rgba);
  const CRGBA &getPenForeground() const { return pen_.fg; }

  void setOffset(int x_offset, int y_offset);

  void resizeEvent(QResizeEvent *);

  void paintEvent(QPaintEvent *);

  virtual void updateImage(const std::string &filename);
  virtual void updateImage(CImagePtr image);

  virtual CImagePtr getImage() const { return image_; }

  void updateVImage(bool image_changed);

  int getImageWidth() const;
  int getImageHeight() const;

  double getZoomFactor() const { return zoom_factor_.value(); }
  void setZoomFactor(double factor);

  bool getFillScreen() const { return fill_screen_; }
  void setFillScreen(bool fill_screen);

  bool getKeepAspect() const { return keep_aspect_; }
  void setKeepAspect(bool keep_aspect);

  bool getZoomCursor() const { return zoom_cursor_active_; }
  void setZoomCursor(bool zoom_cursor);

  double getZoomCursorFactor() const;
  void setZoomCursorFactor(double f);

  int getZoomCursorWidth() const;
  void setZoomCursorWidth(int s);

  void gaussianBlur(double xBlur, double yBlur, int xSize, int ySize);

  void turbulence(bool fractal, double freq, int octaves, int seed);

  void doResize(int width, int height, bool scale, bool aspect);

  void mousePressEvent  (QMouseEvent *event);
  void mouseMoveEvent   (QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

  void keyPressEvent  (QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

  QSize sizeHint() const;

 public slots:
  void selectAll();
  void selectNone();

  void clear();
  void invert();
  void grayscale();
  void sepia();

  void zoomIncrease();
  void zoomDecrease();

 protected slots:
  void doSelect () { setMode(Mode::SELECT ); }
  void doPen    () { setMode(Mode::PEN    ); }
  void doDropper() { setMode(Mode::DROPPER); }
  void doFill   () { setMode(Mode::FILL   ); }

  void resizeTimeout();

 signals:
  void penBgChanged();
  void penFgChanged();

  void keyPressed(const CKeyEvent &event);

 private:
  struct Pen {
    CRGBA fg;
    CRGBA bg;
  };

  typedef CValueHistoryT<double,2> HistDbl;

  CImagePtr              image_;
  CImagePtr              vimage_; // zoomed image
  Mode                   mode_ { Mode::NONE };
  CRGBA                  bg_;
  Pen                    pen_;
  CIPoint2D              press_pos_;
  HistDbl                zoom_factor_ { 1.0 };
  bool                   pressed_ { false };
  bool                   fill_screen_ { false };
  bool                   keep_aspect_ { true };
  bool                   zoom_cursor_active_ { false };
  CAutoPtr<CQZoomCursor> zoom_cursor_;
  CAutoPtr<QTimer>       timer_;
};

#endif
