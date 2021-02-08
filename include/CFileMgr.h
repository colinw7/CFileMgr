#ifndef CFILE_MGR_H
#define CFILE_MGR_H

#include <COrientation.h>
#include <CISize2D.h>
#include <CFileType.h>
#include <CImagePtr.h>
#include <CFileMgrDir.h>
#include <CFileMgrDetails.h>

#include <memory>
#include <map>

class CFileMgrIcons;
class CFileMgrFilmstrip;
class CFileMgrDetails;
class CFileMgrFile;
class CPixelRenderer;
class CConfig;

class CFileMgr {
 public:
  enum ViewType {
    VIEW_ICONS,
    VIEW_FILMSTRIP,
    VIEW_DETAILS
  };

 public:
  CFileMgr();

  virtual ~CFileMgr();

  void setIconsRenderer(CPixelRenderer *renderer);

  void setFilmstripRenderers(CPixelRenderer *image_renderer, CPixelRenderer *icons_renderer);
  void getFilmstripRenderers(CPixelRenderer **image_renderer, CPixelRenderer **icons_renderer);

  void setDetailsRenderer(CPixelRenderer *renderer);

  ViewType getViewType() const { return type_; }

  void setViewType(ViewType type);

  void setIconsView    () { setViewType(VIEW_ICONS    ); }
  void setFilmstripView() { setViewType(VIEW_FILMSTRIP); }
  void setDetailsView  () { setViewType(VIEW_DETAILS  ); }

  CFileMgrIcons     *getIcons    () const { return icons_    .get(); }
  CFileMgrFilmstrip *getFilmstrip() const { return filmstrip_.get(); }
  CFileMgrDetails   *getDetails  () const { return details_  .get(); }

  const std::string &getDirName() const { return dirname_; }

  void setDirName(const std::string &dirname);

  void undoDir();
  void redoDir();

  CFileMgrDir *getDir() const { return dir_.get(); }

  CConfig *getConfig() const { return config_.get(); }

  void setBorder(int border) { border_ = border; }
  int getBorder() const { return border_; }

  const CISize2D &getImageSize() const { return imageSize_; }

  int getImageWidth () const { return imageSize_.width ; }
  int getImageHeight() const { return imageSize_.height; }

  int getIconWidth () const { return iconSize_.width ; }
  int getIconHeight() const { return iconSize_.height; }

  int getFontSize() const;
  void setFontSize(int s);

  int getSmallIconSize() const;
  void setSmallIconSize(int s);

  int getLargeIconSize() const;
  void setLargeIconSize(int s);

  bool getShowDotDot() const { return show_dot_dot_; }
  bool getLargeIcons() const { return large_icons_; }
  bool getSmallIcons() const { return ! getLargeIcons(); }

  void setShowDotDot(bool show_dot_dot = true);
  void setLargeIcons(bool large_icons = true);
  void setSmallIcons(bool small_icons = true);

  bool getShowImages() const { return show_images_; }

  void setShowImages(bool show_images = true);
  void setHideImages(bool hide_images = true);

  bool getShowHidden() const { return show_hidden_; }

  void setShowHidden(bool show_hidden = true);
  void setHideHidden(bool hide_hidden = true);

  bool getForceRGB() const { return force_rgb_; }

  void setForceRGB(bool force_rgb = true);

  CISize2D getSizeForIcon(int iconSize, int char_height) const;

 private:
  void setIconSize();
  void setIconSize(CPixelRenderer *renderer);

  void setDirName1(const std::string &dirname);

 public:
  void openDir (CFileMgrFile *file);
  void openFile(CFileMgrFile *file);

  virtual void redraw();

  virtual void draw();

  virtual void drawIcons();
  virtual void drawFilmstripImage();
  virtual void drawFilmstripIcons();
  virtual void drawDetails();

  virtual void buttonPress  (const CMouseEvent &event);
  virtual void buttonRelease(const CMouseEvent &event);
  virtual void buttonMotion (const CMouseEvent &event);

  virtual void keyPress  (const CKeyEvent &event);
  virtual void keyRelease(const CKeyEvent &event);

  virtual void pointerMotion(const CMouseEvent &event);

  void deselectAll();

  void selectFirst();
  void selectLast();
  void selectNext();
  void selectPrev();

  void select(int x, int y);

  void activate(int x, int y);

  void activateSelected();

  void activateFile(const std::string &filename);
  void activateFile(CFileMgrFile *file);

  void activateFiles(CFileMgrDir::FileList &files);

  virtual void activateDir(const std::string &dirname);

  virtual void activateFileType(const std::string &filename, CFileType type);

 private:
  void activate(CFileMgrFile *file);

 public:
  CFileMgrDir::FileList getSelected();

  void getFileList(CFileMgrDir::FileList &files);

  CFileMgrDir::file_iterator fileBegin();
  CFileMgrDir::file_iterator fileEnd();

  CFileMgrFile *xyToFile(int x, int y) const;

  CImagePtr getImage(CFileType type);
  CImagePtr getImage(const std::string &fileName);

  void scroll();

  virtual void changeDir(const std::string &) { }

  virtual void setTableColumns(const CFileMgrDetails::ColumnList &) { }

  virtual void deleteAllTableRows() { }

  virtual void addTableRow(CFileMgrFile *, const std::vector<std::string> &) { }

  void drawDir (CPixelRenderer *renderer, int x, int y, CFileMgrFile *file);
  void drawFile(CPixelRenderer *renderer, int x, int y, CFileMgrFile *file);

  void drawFileImage (CPixelRenderer *renderer, int x, int y, const CImagePtr &image);
  void drawFileText  (CPixelRenderer *renderer, int x, int y, const std::string &text);
  void drawFileBorder(CPixelRenderer *renderer, int x, int y, bool selected);

 private:
  using ImageMap          = std::map<std::string,CImagePtr>;
  using DirStack          = std::vector<std::string>;
  using FileMgrIconsP     = std::unique_ptr<CFileMgrIcons>;
  using FileMgrFilmstripP = std::unique_ptr<CFileMgrFilmstrip>;
  using FileMgrDetailsP   = std::unique_ptr<CFileMgrDetails>;
  using FileMgrDirP       = std::unique_ptr<CFileMgrDir>;
  using ConfigP           = std::unique_ptr<CConfig>;

  ViewType          type_         { VIEW_ICONS };
  std::string       dirname_;
  FileMgrIconsP     icons_;
  FileMgrFilmstripP filmstrip_;
  FileMgrDetailsP   details_;
  FileMgrDirP       dir_;
  ConfigP           config_;
  int               border_       { 4 };
  CISize2D          imageSize_;
  CISize2D          iconSize_;
  bool              show_dot_dot_ { true };
  bool              large_icons_  { true };
  bool              show_images_  { false };
  bool              show_hidden_  { false };
  bool              force_rgb_    { false };
  DirStack          dir_undo_stack_;
  DirStack          dir_redo_stack_;
};

#endif
