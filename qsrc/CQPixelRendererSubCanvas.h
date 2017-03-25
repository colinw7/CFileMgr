#include <CQPixelRendererCanvas.h>
#include <CQPixelRendererScrolledCanvas.h>

#include <QWidget>

class CQPixelRendererSubCanvas : public CQPixelRendererCanvas {
  Q_OBJECT

 private:
  CQPixelRendererScrolledCanvas *scanvas_;

 public:
  CQPixelRendererSubCanvas(QWidget *parent,
                           CQPixelRendererScrolledCanvas *scanvas) :
   CQPixelRendererCanvas(parent), scanvas_(scanvas) {
  }

 ~CQPixelRendererSubCanvas() { }

  virtual void draw() {
    scanvas_->draw();
  }

  virtual void resize() {
    scanvas_->updateScrollbars();

    scanvas_->resize();
  }

  virtual void mousePress(const CMouseEvent &event) {
    scanvas_->mousePress(event);
  }

  virtual void mouseMotion(const CMouseEvent &event) {
    scanvas_->mouseMotion(event);
  }

  virtual void mouseRelease(const CMouseEvent &event) {
    scanvas_->mouseRelease(event);
  }

  virtual void pointerMotion(const CMouseEvent &event) {
    scanvas_->pointerMotion(event);
  }

  virtual void selectionNotify(CButtonAction action) {
    scanvas_->selectionNotify(action);
  }

  virtual void keyPress(const CKeyEvent &event) {
    scanvas_->keyPress(event);
  }

  virtual void keyRelease(const CKeyEvent &event) {
    scanvas_->keyRelease(event);
  }

  virtual void scroll(int delta) {
    scanvas_->scroll(delta);
  }

 private slots:
  void scrolledSlot() {
    scanvas_->scrolledSlot();
  }
};
