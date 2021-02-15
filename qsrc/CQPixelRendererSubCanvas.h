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

  void draw() override {
    scanvas_->draw();
  }

  void resize() override {
    scanvas_->updateScrollbars();

    scanvas_->resize();
  }

  void mousePress(const CMouseEvent &event) override {
    scanvas_->mousePress(event);
  }

  void mouseMotion(const CMouseEvent &event) override {
    scanvas_->mouseMotion(event);
  }

  void mouseRelease(const CMouseEvent &event) override {
    scanvas_->mouseRelease(event);
  }

  void pointerMotion(const CMouseEvent &event) override {
    scanvas_->pointerMotion(event);
  }

  void selectionNotify(CButtonAction action) override {
    scanvas_->selectionNotify(action);
  }

  void keyPress(const CKeyEvent &event) override {
    scanvas_->keyPress(event);
  }

  void keyRelease(const CKeyEvent &event) override {
    scanvas_->keyRelease(event);
  }

  void scroll(int delta) override {
    scanvas_->scroll(delta);
  }

 private slots:
  void scrolledSlot() {
    scanvas_->scrolledSlot();
  }
};
