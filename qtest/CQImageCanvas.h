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

 public:
  enum Mode {
    MODE_NONE,
    MODE_SELECT,
    MODE_PEN,
    MODE_DROPPER,
    MODE_FILL
  };

 public:
  CQImageCanvas(QWidget *parent=0);

  virtual ~CQImageCanvas();

  void addTools();

  void setMode(Mode mode);

  void setBackground(const CRGBA &rgba);

  void setPenBackground(const CRGBA &rgba);
  void setPenForeground(const CRGBA &rgba);

  const CRGBA &getPenBackground() const { return pen_.bg; }
  const CRGBA &getPenForeground() const { return pen_.fg; }

  double getZoomFactor() const { return zoom_factor_.value(); }

  void setZoomFactor(double factor);

  void setOffset(int x_offset, int y_offset);

  void resizeEvent(QResizeEvent *);

  void paintEvent(QPaintEvent *);

  virtual void updateImage(const std::string &filename);
  virtual void updateImage(CImagePtr image);

  virtual CImagePtr getImage() const { return image_; }

  void updateVImage(bool image_changed);

  int getImageWidth() const;
  int getImageHeight() const;

  void clear();
  void invert();
  void grayscale();
  void sepia();
  void gaussianBlur(double xBlur, double yBlur, int xSize, int ySize);
  void turbulence(bool fractal, double freq, int octaves, int seed);
  void doResize(int width, int height, bool scale, bool aspect);

  void setFillScreen(bool fill_screen);
  void setKeepAspect(bool keep_aspect);
  void setZoomCursor(bool zoom_cursor);

  bool getFillScreen() const { return fill_screen_; }
  bool getKeepAspect() const { return keep_aspect_; }
  bool getZoomCursor() const { return zoom_cursor_; }

  void selectAll();
  void selectNone();

  void zoomIncrease();
  void zoomDecrease();

  void mousePressEvent  (QMouseEvent *event);
  void mouseMoveEvent   (QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

  void keyPressEvent  (QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

  QSize sizeHint() const;

 protected slots:
  void doSelect () { setMode(MODE_SELECT ); }
  void doPen    () { setMode(MODE_PEN    ); }
  void doDropper() { setMode(MODE_DROPPER); }
  void doFill   () { setMode(MODE_FILL   ); }

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
  Mode                   mode_;
  CRGBA                  bg_;
  Pen                    pen_;
  CIPoint2D              press_pos_;
  HistDbl                zoom_factor_;
  bool                   pressed_;
  bool                   fill_screen_;
  bool                   keep_aspect_;
  bool                   zoom_cursor_active_;
  CAutoPtr<CQZoomCursor> zoom_cursor_;
  CAutoPtr<QTimer>       timer_;
};

#endif
