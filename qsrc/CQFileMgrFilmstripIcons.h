#ifndef CQFileMgrFilmstripIcons_H
#define CQFileMgrFilmstripIcons_H

#include <CQPixelRendererScrolledCanvas.h>
#include <CEvent.h>

class CQFileMgr;
class CQWidgetPixelRenderer;
class QContextMenuEvent;

class CQFileMgrFilmstripIcons : public CQPixelRendererScrolledCanvas {
  Q_OBJECT

 public:
  CQFileMgrFilmstripIcons(QWidget *parent, CQFileMgr *filemgr);

  virtual ~CQFileMgrFilmstripIcons();

  void setOffset(int x_offset, int y_offset);

  void draw  () override;
  void resize() override;

  void selectAll();
  void selectNone();

  void scrolled();

  void mousePress  (const CMouseEvent &event) override;
  void mouseMotion (const CMouseEvent &event) override;
  void mouseRelease(const CMouseEvent &event) override;

  void pointerMotion(const CMouseEvent &event) override;

  void keyPress  (const CKeyEvent &event) override;
  void keyRelease(const CKeyEvent &event) override;

  void selectionNotify(CButtonAction action) override;

 private:
  void updateSize();

 private:
  void contextMenuEvent(QContextMenuEvent *event);

 private:
  CQFileMgr *filemgr_ { nullptr };
};

#endif
