#include <CFileMgrFilmstrip.h>
#include <CFileMgr.h>
#include <CFileMgrDir.h>
#include <CFileMgrFile.h>

#include <CFileUtil.h>
#include <CImageLib.h>
#include <CPixelRenderer.h>
#include <CEvent.h>

CFileMgrFilmstrip::
CFileMgrFilmstrip(CFileMgr *file_mgr) :
 file_mgr_(file_mgr)
{
  CImage::setResizeType(CIMAGE_RESIZE_BILINEAR);
}

CFileMgrFilmstrip::
~CFileMgrFilmstrip()
{
}

void
CFileMgrFilmstrip::
setRenderers(CPixelRenderer *image_renderer, CPixelRenderer *icons_renderer)
{
  setImageRenderer(image_renderer);
  setIconsRenderer(icons_renderer);
}

void
CFileMgrFilmstrip::
setImageRenderer(CPixelRenderer *renderer)
{
  image_renderer_ = renderer;
}

void
CFileMgrFilmstrip::
setIconsRenderer(CPixelRenderer *renderer)
{
  icons_renderer_ = renderer;

  updateFont();
}

void
CFileMgrFilmstrip::
updateFont()
{
  auto font = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL, file_mgr_->getFontSize());

  if (icons_renderer_ != 0)
    icons_renderer_->setFont(font);
}

void
CFileMgrFilmstrip::
redraw()
{
  file_mgr_->redraw();
}

void
CFileMgrFilmstrip::
draw()
{
  drawImage();

  drawIcons();
}

void
CFileMgrFilmstrip::
drawImage()
{
  if (! image_renderer_)
    return;

  //-----

  int icon_height = file_mgr_->getIconHeight();

  int width  = image_renderer_->getWidth ();
  int height = image_renderer_->getHeight();

  if (icons_renderer_ == image_renderer_)
    height -= icon_height;

  //-----

  // Draw Current Image
  CFileMgrDir::FileList selected = file_mgr_->getSelected();

  CFileMgrFile *file = 0;

  if (! selected.empty())
    file = *selected.begin();

  if (file != 0 && height > 0) {
    CImagePtr image = file->getImage(width, height);

    if (image.isValid()) {
      int x = (width  - (int) image->getWidth ())/2;
      int y = (height - (int) image->getHeight())/2;

      image_renderer_->drawAlphaImage(CIPoint2D(x, y), image);
    }
  }
}

void
CFileMgrFilmstrip::
drawIcons()
{
  if (! icons_renderer_)
    return;

  //-----

  CFileMgrDir::FileList file_list;

  file_mgr_->getFileList(file_list);

  //-----

  int icon_height = file_mgr_->getIconHeight();

//int width  = icons_renderer_->getWidth ();
  int height = icons_renderer_->getHeight();

  //-----

  // Draw Icons
  int x = 0, y = 0;

  if (icons_renderer_ == image_renderer_)
    y = height - icon_height;

  CFileMgrDir::file_iterator p1 = file_list.begin();
  CFileMgrDir::file_iterator p2 = file_list.end  ();

  for ( ; p1 != p2; ++p1)
    stepPosition(&x);

  size_.width = x;

  //-----

  if (icons_renderer_ != image_renderer_)
    icons_renderer_->clear();

  //-----

  x = 0;
  y = height - icon_height;

  CIPoint2D p(x, y);

  icons_renderer_->applyOffset(p);

  p1 = file_list.begin();

  for ( ; p1 != p2; ++p1) {
    if ((*p1)->isDirectory())
      file_mgr_->drawDir(icons_renderer_, p.x, p.y, *p1);
    else
      file_mgr_->drawFile(icons_renderer_, p.x, p.y, *p1);

    stepPosition(&p.x);
  }
}

void
CFileMgrFilmstrip::
stepPosition(int *x)
{
  *x += file_mgr_->getIconWidth();
}

void
CFileMgrFilmstrip::
keyPress(const CKeyEvent &event)
{
  const std::string &str = event.getText();

  if      (str == "n")
    file_mgr_->selectNext();
  else if (str == "p")
    file_mgr_->selectPrev();

  redraw();
}

void
CFileMgrFilmstrip::
keyRelease(const CKeyEvent &)
{
}
