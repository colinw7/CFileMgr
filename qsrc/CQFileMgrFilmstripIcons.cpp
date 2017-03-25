#include <CQFileMgrFilmstripIcons.h>
#include <CQPixelRendererCanvas.h>
#include <CQWidgetPixelRenderer.h>
#include <CQFileMgr.h>
#include <CQUtil.h>
#include <CFileMgrLib.h>
#include <CEvent.h>

#include <QContextMenuEvent>

CQFileMgrFilmstripIcons::
CQFileMgrFilmstripIcons(QWidget *parent, CQFileMgr *filemgr) :
 CQPixelRendererScrolledCanvas(parent), filemgr_(filemgr)
{
  setObjectName("fileMgrFilmstripIcons");

  allowSelection();

  showVBar(false);

  CPixelRenderer *image_renderer, *icons_renderer;

  filemgr_->getFileMgr()->getFilmstripRenderers(&image_renderer, &icons_renderer);

  filemgr_->getFileMgr()->setFilmstripRenderers(image_renderer, getRenderer());

  updateSize();
}

CQFileMgrFilmstripIcons::
~CQFileMgrFilmstripIcons()
{
}

void
CQFileMgrFilmstripIcons::
setOffset(int x_offset, int y_offset)
{
  CIPoint2D offset(x_offset, y_offset);

  repaint();
}

void
CQFileMgrFilmstripIcons::
resize()
{
  filemgr_->resize();

  updateSize();
}

void
CQFileMgrFilmstripIcons::
draw()
{
  getRenderer()->setBackground(CRGB(1,1,1));

  getRenderer()->clear();

  filemgr_->drawFilmstripIcons();

  updateSize();
}

void
CQFileMgrFilmstripIcons::
scrolled()
{
}

void
CQFileMgrFilmstripIcons::
mousePress(const CMouseEvent &)
{
}

void
CQFileMgrFilmstripIcons::
mouseMotion(const CMouseEvent &event)
{
  pointerMotion(event);
}

void
CQFileMgrFilmstripIcons::
mouseRelease(const CMouseEvent &event)
{
  if (event.getCount() == 2 && event.getButton() == CBUTTON_LEFT)
    filemgr_->activate(event.getPosition());
}

void
CQFileMgrFilmstripIcons::
pointerMotion(const CMouseEvent &)
{
}

void
CQFileMgrFilmstripIcons::
keyPress(const CKeyEvent &event)
{
  filemgr_->keyPress(event);
}

void
CQFileMgrFilmstripIcons::
keyRelease(const CKeyEvent &event)
{
  filemgr_->keyRelease(event);
}

void
CQFileMgrFilmstripIcons::
selectionNotify(CButtonAction action)
{
  if (action != CBUTTON_ACTION_RELEASE)
    return;

  const CIBBox2D &selection = getSelectionRect();

  filemgr_->selectInside(selection);

  clearSelection();
}

void
CQFileMgrFilmstripIcons::
updateSize()
{
  CISize2D size(filemgr_->getContentsWidth() - 1, getCanvas()->height() - 1);

  setVSize(size);

  updateScrollbars();
}

void
CQFileMgrFilmstripIcons::
contextMenuEvent(QContextMenuEvent *event)
{
  filemgr_->popupMenu(this, event->globalPos().x(), event->globalPos().y());
}
