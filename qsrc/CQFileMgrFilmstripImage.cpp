#include <CQFileMgrFilmstripImage.h>
#include <CQWidgetPixelRenderer.h>
#include <CFileMgrIcons.h>
#include <CQFileMgr.h>

#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>

CQFileMgrFilmstripImage::
CQFileMgrFilmstripImage(QWidget *parent, CQFileMgr *filemgr) :
 CQPixelRendererScrolledCanvas(parent), filemgr_(filemgr)
{
  setObjectName("fileMgrFilmstripImage");

  CPixelRenderer *image_renderer, *icons_renderer;

  filemgr_->getFileMgr()->getFilmstripRenderers(&image_renderer, &icons_renderer);

  filemgr_->getFileMgr()->setFilmstripRenderers(getRenderer(), icons_renderer);
}

CQFileMgrFilmstripImage::
~CQFileMgrFilmstripImage()
{
}

void
CQFileMgrFilmstripImage::
resize()
{
  filemgr_->resize();
}

void
CQFileMgrFilmstripImage::
draw()
{
  getRenderer()->setBackground(CRGB(1,1,1));

  getRenderer()->clear();

  filemgr_->drawFilmstripImage();
}

void
CQFileMgrFilmstripImage::
contextMenuEvent(QContextMenuEvent *event)
{
  static QAction *iconsAction;
  static QAction *detailsAction;
  static QAction *smallIconsAction;
  static QAction *showImagesAction;
  static QAction *showHiddenAction;

  if (! showImagesAction) {
    iconsAction      = new QAction(tr("&Icons View"  ), this);
    detailsAction    = new QAction(tr("&Details View"), this);
    smallIconsAction = new QAction(tr("Small I&cons" ), this);
    showImagesAction = new QAction(tr("Show &Images" ), this);
    showHiddenAction = new QAction(tr("Show &Hidden" ), this);

    smallIconsAction->setCheckable(true);
    showImagesAction->setCheckable(true);
    showHiddenAction->setCheckable(true);

    connect(iconsAction     , SIGNAL(triggered())  , this, SLOT(iconsView()));
    connect(detailsAction   , SIGNAL(triggered())  , this, SLOT(detailsView()));
    connect(showImagesAction, SIGNAL(toggled(bool)), this, SLOT(showImages(bool)));
    connect(smallIconsAction, SIGNAL(toggled(bool)), this, SLOT(smallIcons(bool)));
    connect(showHiddenAction, SIGNAL(toggled(bool)), this, SLOT(showHidden(bool)));
  }

  QMenu menu(this);

  smallIconsAction->setChecked(filemgr_->getSmallIcons());
  showImagesAction->setChecked(filemgr_->getShowImages());
  showHiddenAction->setChecked(filemgr_->getShowHidden());

  menu.addAction(iconsAction);
  menu.addAction(detailsAction);

  menu.addAction(smallIconsAction);
  menu.addAction(showImagesAction);
  menu.addAction(showHiddenAction);

  menu.exec(event->globalPos());
}

void
CQFileMgrFilmstripImage::
iconsView()
{
  filemgr_->setIconsView();
}

void
CQFileMgrFilmstripImage::
detailsView()
{
  filemgr_->setDetailsView();
}

void
CQFileMgrFilmstripImage::
showImages(bool flag)
{
  filemgr_->setShowImages(flag);
}

void
CQFileMgrFilmstripImage::
smallIcons(bool flag)
{
  filemgr_->setSmallIcons(flag);
}

void
CQFileMgrFilmstripImage::
showHidden(bool flag)
{
  filemgr_->setShowHidden(flag);
}
