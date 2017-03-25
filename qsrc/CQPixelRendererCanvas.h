#ifndef CQT_PIXEL_RENDERER_CANVAS_H
#define CQT_PIXEL_RENDERER_CANVAS_H

#include <QWidget>

#include <CIPoint2D.h>
#include <CISize2D.h>
#include <CIBBox2D.h>
#include <CEvent.h>

class CQWidgetPixelRenderer;
class QRubberBand;
class CMouseEvent;
class CKeyEvent;

class CQPixelRendererCanvas : public QWidget {
  Q_OBJECT

 public:
  CQPixelRendererCanvas(QWidget *parent = 0);
 ~CQPixelRendererCanvas();

  CQWidgetPixelRenderer *getRenderer() const { return renderer_; }

  void allowSelection(bool allow_selection=true, bool show_rubber_band=true);

  const CIBBox2D &getSelectionRect() const { return selection_; }

  void setSelectionRect(const CIBBox2D &rect);
  void resetSelectionRect();

  void setOffset(const CIPoint2D &offset);

  const CIPoint2D &getMousePos() const { return mouse_pos_; }

  const CIPoint2D &getOffset() const;

  void setVSize(const CISize2D &vsize);

  const CISize2D &getVSize() const;

  void trackMouse(bool flag);

  void setContentsChanged(bool flag = true);

  void signalChanged();

  void setClear(bool clear) { clear_ = clear; }

  virtual void draw  () { }
  virtual void resize() { }

  virtual void mousePress  (const CMouseEvent &) { }
  virtual void mouseMotion (const CMouseEvent &) { }
  virtual void mouseRelease(const CMouseEvent &) { }

  virtual void pointerMotion(const CMouseEvent &) { }

  virtual void keyPress  (const CKeyEvent &) { }
  virtual void keyRelease(const CKeyEvent &) { }

  virtual void selectionNotify(CButtonAction) { }

  virtual void scroll(int) { }

  void scrolled();

  void clearSelection();

  void applyOffset(CIPoint2D &point);
  void unapplyOffset(CIPoint2D &point);

 protected:
  void paintEvent (QPaintEvent  *event);
  void resizeEvent(QResizeEvent *event);

  void mousePressEvent  (QMouseEvent *event);
  void mouseMoveEvent   (QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

  void mouseDoubleClickEvent(QMouseEvent *event);

  void wheelEvent(QWheelEvent *event);

  void keyPressEvent  (QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

  void updateRubberband();

 protected:
  CQWidgetPixelRenderer *renderer_ { 0 };
  bool                   pressed_ { false };
  CIPoint2D              press_pos_;
  CMouseButton           press_button_;
  bool                   allow_selection_ { false };
  CIBBox2D               selection_;
  bool                   show_rubber_band_ { false };
  QRubberBand           *rubber_band_ { 0 };
  CIPoint2D              mouse_pos_ { -1, -1 };
  bool                   clear_ { true };
};

#endif
