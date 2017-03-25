#ifndef CQT_PIXEL_RENDERER_SCROLLED_CANVAS_H
#define CQT_PIXEL_RENDERER_SCROLLED_CANVAS_H

#include <CISize2D.h>
#include <CIBBox2D.h>
#include <CEvent.h>
#include <CScrollType.h>

#include <QScrollBar>

#include <accessor.h>

class CQWidgetPixelRenderer;
class CQPixelRendererCanvas;
class CQPixelRendererSubCanvas;
class CMouseEvent;
class CKeyEvent;

class CQPixelRendererScrolledCanvas : public QWidget {
  Q_OBJECT

 public:
  CQPixelRendererScrolledCanvas(QWidget *parent=0);

  virtual ~CQPixelRendererScrolledCanvas();

  CQPixelRendererCanvas *getCanvas() const;

  QScrollBar *getHScroll() const;
  QScrollBar *getVScroll() const;

  ACCESSOR(HStep, uint, hstep)
  ACCESSOR(VStep, uint, vstep)

  CQWidgetPixelRenderer *getRenderer();

  const CIPoint2D &getMousePos();

  void allowSelection();

  void setContentsChanged(bool flag = true);

  void signalChanged();

  const CIBBox2D &getSelectionRect();

  void setSelectionRect(const CIBBox2D &rect);
  void resetSelectionRect();

  void setVSize(const CISize2D &size);
  const CISize2D &getVSize() const;

  void showHBar(bool flag);
  void showVBar(bool flag);

  void trackMouse(bool flag);

  virtual void draw  () { }
  virtual void resize() { }

  void clearSelection();

  void scroll(int delta);

  void scrollTo(const CIPoint2D &point, CScrollType pos);
  void scrollTo(const CIBBox2D &rect, CScrollType pos);

  void applyOffset(CIPoint2D &point);
  void unapplyOffset(CIPoint2D &point);

  const CIPoint2D &getOffset() const;

  void scrolledSlot();

 public:
  virtual void scrolled() { }

  virtual void mousePress  (const CMouseEvent &) { }
  virtual void mouseMotion (const CMouseEvent &) { }
  virtual void mouseRelease(const CMouseEvent &) { }

  virtual void pointerMotion(const CMouseEvent &) { }

  virtual void keyPress  (const CKeyEvent &) { }
  virtual void keyRelease(const CKeyEvent &) { }

  virtual void selectionNotify(CButtonAction) { }

 protected:
  friend class CQPixelRendererSubCanvas;

  void updateOffset();
  void updateScrollbars();

 private:
  CQPixelRendererSubCanvas *canvas_;
  QScrollBar               *hscroll_;
  QScrollBar               *vscroll_;
  uint                      hstep_, vstep_;
};

#endif
