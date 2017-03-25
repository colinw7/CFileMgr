#include <CQFileMgrIcons.h>
#include <CQFileMgr.h>
#include <CQPixelRendererCanvas.h>
#include <CQWidgetPixelRenderer.h>
#include <CQUtil.h>
#include <CFileMgrLib.h>
#include <CEvent.h>
#include <QContextMenuEvent>

// CQPixelRendererScrolledCanvas for allowSelection, showHBar

CQFileMgrIcons::
CQFileMgrIcons(QWidget *parent, CQFileMgr *filemgr) :
 CQPixelRendererScrolledCanvas(parent), filemgr_(filemgr)
{
  setObjectName("fileMgrIcons");

  allowSelection();

  showHBar(false);

  filemgr_->getFileMgr()->setIconsRenderer(getRenderer());

  updateSize();
}

CQFileMgrIcons::
~CQFileMgrIcons()
{
}

void
CQFileMgrIcons::
setOffset(int x_offset, int y_offset)
{
  CIPoint2D offset(x_offset, y_offset);

  repaint();
}

void
CQFileMgrIcons::
resize()
{
  filemgr_->resize();

  updateSize();
}

void
CQFileMgrIcons::
draw()
{
  getRenderer()->setBackground(CRGB(1,1,1));

  getRenderer()->clear();

  filemgr_->drawIcons();

  updateSize();
}

void
CQFileMgrIcons::
scrolled()
{
}

void
CQFileMgrIcons::
mousePress(const CMouseEvent &)
{
}

void
CQFileMgrIcons::
mouseMotion(const CMouseEvent &event)
{
  pointerMotion(event);
}

void
CQFileMgrIcons::
mouseRelease(const CMouseEvent &event)
{
  if (event.getCount() == 2 && event.getButton() == CBUTTON_LEFT)
    filemgr_->activate(event.getPosition());
}

void
CQFileMgrIcons::
pointerMotion(const CMouseEvent &)
{
}

void
CQFileMgrIcons::
keyPress(const CKeyEvent &event)
{
  filemgr_->keyPress(event);
}

void
CQFileMgrIcons::
keyRelease(const CKeyEvent &event)
{
  filemgr_->keyRelease(event);
}

void
CQFileMgrIcons::
selectionNotify(CButtonAction action)
{
  if (action != CBUTTON_ACTION_RELEASE)
    return;

  const CIBBox2D &selection = getSelectionRect();

  filemgr_->selectInside(selection);

  clearSelection();
}

void
CQFileMgrIcons::
updateSize()
{
  CISize2D size(getCanvas()->width() - 1, filemgr_->getContentsHeight() - 1);

  setVSize(size);

  updateScrollbars();
}

void
CQFileMgrIcons::
contextMenuEvent(QContextMenuEvent *event)
{
  filemgr_->deselectAll();

  CFileMgrFile *file = filemgr_->getFileMgr()->xyToFile(event->pos().x(), event->pos().y());

  if (file)
    file->setSelected(true);

  signalChanged();

  filemgr_->popupMenu(this, event->globalPos().x(), event->globalPos().y());
}
