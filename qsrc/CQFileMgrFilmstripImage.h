#ifndef CQFileMgrFilmstripImage_H
#define CQFileMgrFilmstripImage_H

#include <CQPixelRendererScrolledCanvas.h>

class CQFileMgr;
class CQWidgetPixelRenderer;
class QContextMenuEvent;

class CQFileMgrFilmstripImage : public CQPixelRendererScrolledCanvas {
  Q_OBJECT

 public:
  CQFileMgrFilmstripImage(QWidget *parent, CQFileMgr *filemgr);

  virtual ~CQFileMgrFilmstripImage();

  void draw  () override;
  void resize() override;

 private slots:
  void iconsView();
  void detailsView();
  void showImages(bool flag);
  void smallIcons(bool flag);
  void showHidden(bool flag);

 private:
  void contextMenuEvent(QContextMenuEvent *event);

 private:
  CQFileMgr *filemgr_ { nullptr };
};

#endif
