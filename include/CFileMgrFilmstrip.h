#ifndef CFILE_MGR_FILMSTRIP_H
#define CFILE_MGR_FILMSTRIP_H

#include <CFile.h>
#include <CEvent.h>
#include <CISize2D.h>

class CFileMgr;
class CPixelRenderer;
class CFileMgrFile;

class CFileMgrFilmstrip {
 public:
  CFileMgrFilmstrip(CFileMgr *file_mgr);

 ~CFileMgrFilmstrip();

  CFileMgr *getFileMgr() const { return file_mgr_; }

  void setRenderers(CPixelRenderer *image_renderer,
                    CPixelRenderer *icons_renderer);

  void setImageRenderer(CPixelRenderer *renderer);
  void setIconsRenderer(CPixelRenderer *renderer);

  CPixelRenderer *getImageRenderer() const { return image_renderer_; }
  CPixelRenderer *getIconsRenderer() const { return icons_renderer_; }

  int getWidth () const { return size_.width; }
  int getHeight() const { return size_.height; }

  void redraw();

  void draw();

  void drawImage();
  void drawIcons();

  void keyPress  (const CKeyEvent &event);
  void keyRelease(const CKeyEvent &event);

 private:
  void stepPosition(int *x);

 private:
  CFileMgr       *file_mgr_ { nullptr };
  CPixelRenderer *image_renderer_ { nullptr };
  CPixelRenderer *icons_renderer_ { nullptr };
  CISize2D        size_;
};

#endif
