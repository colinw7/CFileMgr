#include <CFileMgr.h>
#include <CFileMgrIcons.h>
#include <CFileMgrFilmstrip.h>
#include <CFileMgrDetails.h>
#include <CFileMgrDir.h>
#include <CFileMgrFile.h>

#include <CStrUtil.h>
#include <CMime.h>
#include <CFileUtil.h>
#include <CConfig.h>
#include <CCommandMgr.h>
#include <CPixelRenderer.h>
#include <CImageLib.h>
#include <CFileIcon.h>
#include <CEvent.h>
#include <COSFile.h>

CFileMgr::
CFileMgr()
{
  config_ = std::make_unique<CConfig>("CFileMgr");

  dirname_ = COSFile::getCurrentDir();

  icons_     = std::make_unique<CFileMgrIcons    >(this);
  filmstrip_ = std::make_unique<CFileMgrFilmstrip>(this);
  details_   = std::make_unique<CFileMgrDetails  >(this);

  dir_ = std::make_unique<CFileMgrDir>(this);

  setIconSize();
}

CFileMgr::
~CFileMgr()
{
}

void
CFileMgr::
setIconsRenderer(CPixelRenderer *renderer)
{
  icons_->setRenderer(renderer);

  setIconSize();
}

void
CFileMgr::
setFilmstripRenderers(CPixelRenderer *image_renderer, CPixelRenderer *icons_renderer)
{
  filmstrip_->setRenderers(image_renderer, icons_renderer);

  setIconSize();
}

void
CFileMgr::
getFilmstripRenderers(CPixelRenderer **image_renderer, CPixelRenderer **icons_renderer)
{
  *image_renderer = filmstrip_->getImageRenderer();
  *icons_renderer = filmstrip_->getIconsRenderer();
}

void
CFileMgr::
setDetailsRenderer(CPixelRenderer *renderer)
{
  details_->setRenderer(renderer);

  setIconSize();
}

void
CFileMgr::
setViewType(ViewType type)
{
  if (type == type_)
    return;

  type_ = type;

  setIconSize();

  redraw();
}

void
CFileMgr::
setDirName(const std::string &dirname)
{
  if (dirname == dirname_)
    return;

  dir_redo_stack_.clear();
  dir_undo_stack_.push_back(dirname_);

  setDirName1(dirname);
}

void
CFileMgr::
setDirName1(const std::string &dirname)
{
  dirname_ = dirname;

  getDir()->reload(getDirName());

  details_->loadDir();

  redraw();
}

void
CFileMgr::
undoDir()
{
  if (dir_undo_stack_.empty())
    return;

  std::string dirname = dir_undo_stack_.back();

  dir_undo_stack_.pop_back();
  dir_redo_stack_.push_back(dirname_);

  setDirName1(dirname);
}

void
CFileMgr::
redoDir()
{
  if (dir_redo_stack_.empty())
    return;

  std::string dirname = dir_redo_stack_.back();

  dir_redo_stack_.pop_back();
  dir_undo_stack_.push_back(dirname_);

  setDirName1(dirname);
}

void
CFileMgr::
setForceRGB(bool force_rgb)
{
  force_rgb_ = force_rgb;

  redraw();
}

void
CFileMgr::
setShowDotDot(bool show_dot_dot)
{
  if (show_dot_dot == show_dot_dot_)
    return;

  show_dot_dot_ = show_dot_dot;

  getDir()->reload(getDirName(), true);

  redraw();
}

void
CFileMgr::
setLargeIcons(bool large_icons)
{
  if (large_icons == large_icons_)
    return;

  large_icons_ = large_icons;

  CFileIconMgrInst->setIconType(
    large_icons_ ? CFileIconMgr::LARGE_ICONS : CFileIconMgr::SMALL_ICONS);

  setIconSize();

  redraw();
}

void
CFileMgr::
setSmallIcons(bool small_icons)
{
  setLargeIcons(! small_icons);
}

void
CFileMgr::
setShowImages(bool show_images)
{
  if (show_images == show_images_)
    return;

  show_images_ = show_images;

  CFileIconMgrInst->setShowImage(show_images_);

  redraw();
}

void
CFileMgr::
setHideImages(bool hide_images)
{
  setShowImages(! hide_images);
}

void
CFileMgr::
setShowHidden(bool show_hidden)
{
  if (show_hidden != show_hidden_) {
    show_hidden_ = show_hidden;

    redraw();
  }
}

void
CFileMgr::
setHideHidden(bool hide_hidden)
{
  setShowHidden(! hide_hidden);
}

void
CFileMgr::
setIconSize()
{
  if      (getViewType() == VIEW_ICONS)
    setIconSize(icons_->getRenderer());
  else if (getViewType() == VIEW_FILMSTRIP)
    setIconSize(filmstrip_->getIconsRenderer());
}

void
CFileMgr::
setIconSize(CPixelRenderer *renderer)
{
  int iconSize;

  if (getLargeIcons())
    iconSize = getLargeIconSize();
  else
    iconSize = getSmallIconSize();

  imageSize_.set(iconSize, iconSize);

  int char_height = 8;

  if (renderer)
    char_height = renderer->getCharHeight();

  iconSize_ = getSizeForIcon(iconSize, char_height);
}

CISize2D
CFileMgr::
getSizeForIcon(int iconSize, int char_height) const
{
  CISize2D size;

  size.width  = int(iconSize*1.25) + 4*border_;
  size.height = iconSize + 5*border_ + char_height;

  return size;
}

int
CFileMgr::
getSmallIconSize() const
{
  int iconSize;

  if (! config_->getValue("smallIconSize", &iconSize))
    iconSize = 32;

  return iconSize;
}

void
CFileMgr::
setSmallIconSize(int s)
{
  config_->setValue("smallIconSize", s);

  CFileIconMgrInst->setSmallIconSize(s);
}

int
CFileMgr::
getLargeIconSize() const
{
  int iconSize;

  if (! config_->getValue("largeIconSize", &iconSize))
    iconSize = 64;

  return iconSize;
}

void
CFileMgr::
setLargeIconSize(int s)
{
  config_->setValue("largeIconSize", s);

  CFileIconMgrInst->setLargeIconSize(s);
}

int
CFileMgr::
getFontSize() const
{
  int fontSize;

  if (! config_->getValue("fontSize", &fontSize))
    fontSize = 8;

  return fontSize;
}

void
CFileMgr::
setFontSize(int s)
{
  config_->setValue("fontSize", s);

  icons_    ->updateFont();
  filmstrip_->updateFont();
  details_  ->updateFont();

  setIconSize();
}

void
CFileMgr::
openDir(CFileMgrFile *file)
{
  activateDir(file->getPathName());
}

void
CFileMgr::
openFile(CFileMgrFile *file)
{
  activateFile(file->getFileName());
}

void
CFileMgr::
redraw()
{
}

void
CFileMgr::
draw()
{
  getDir()->reload(getDirName());

  if      (getViewType() == VIEW_ICONS)
    drawIcons();
  else if (getViewType() == VIEW_FILMSTRIP) {
    drawFilmstripImage();

    drawFilmstripIcons();
  }
  else
    drawDetails();
}

void
CFileMgr::
drawIcons()
{
  icons_->draw();
}

void
CFileMgr::
drawFilmstripImage()
{
  filmstrip_->drawImage();
}

void
CFileMgr::
drawFilmstripIcons()
{
  filmstrip_->drawIcons();
}

void
CFileMgr::
drawDetails()
{
  details_->draw();
}

void
CFileMgr::
buttonPress(const CMouseEvent &event)
{
  if      (getViewType() == VIEW_ICONS) {
    bool multi_select = true;

    if (! multi_select || (! event.isControlKey() && ! event.isShiftKey()))
      deselectAll();

    select(event.getX(), event.getY());

    if (event.getCount() > 1)
      activateSelected();
  }
  else if (getViewType() == VIEW_FILMSTRIP) {
    bool multi_select = false;

    if (! multi_select || (! event.isControlKey() && ! event.isShiftKey()))
      deselectAll();

    select(event.getX(), event.getY());

    if (event.getCount() > 1)
      activateSelected();
  }
  else if (getViewType() == VIEW_DETAILS)
    details_->buttonPress(event);
}

void
CFileMgr::
buttonRelease(const CMouseEvent &event)
{
  if (getViewType() == VIEW_DETAILS)
    details_->buttonRelease(event);
}

void
CFileMgr::
buttonMotion(const CMouseEvent &event)
{
  if (getViewType() == VIEW_DETAILS)
    details_->buttonMotion(event);
}

void
CFileMgr::
keyPress(const CKeyEvent &event)
{
  if      (getViewType() == VIEW_ICONS)
    icons_->keyPress(event);
  else if (getViewType() == VIEW_FILMSTRIP)
    filmstrip_->keyPress(event);
  else
    details_->keyPress(event);
}

void
CFileMgr::
keyRelease(const CKeyEvent &event)
{
  const std::string &str = event.getText();

  if (event.isAltKey()) {
    if      (str == "l") {
      setViewType(VIEW_ICONS);

      return;
    }
    else if (str == "f") {
      setViewType(VIEW_FILMSTRIP);

      return;
    }
    else if (str == "d") {
      setViewType(VIEW_DETAILS);

      return;
    }
  }

  if      (getViewType() == VIEW_ICONS)
    icons_->keyRelease(event);
  else if (getViewType() == VIEW_FILMSTRIP)
    filmstrip_->keyRelease(event);
  else
    details_->keyRelease(event);
}

void
CFileMgr::
pointerMotion(const CMouseEvent &event)
{
  if (getViewType() == VIEW_DETAILS)
    details_->pointerMotion(event);
}

void
CFileMgr::
deselectAll()
{
  getDir()->deselectAll();
}

void
CFileMgr::
select(int x, int y)
{
  CFileMgrFile *file = xyToFile(x, y);

  if (file != nullptr)
    file->setSelected(true);

  redraw();
}

void
CFileMgr::
selectFirst()
{
  CFileMgrDir::FileList file_list;

  getFileList(file_list);

  //-----

  CFileMgrDir::file_iterator ps = file_list.begin();
  CFileMgrDir::file_iterator pe = file_list.end();

  if (ps == pe)
    return;

  (*ps)->setSelected(true);
}

void
CFileMgr::
selectLast()
{
  CFileMgrDir::FileList file_list;

  getFileList(file_list);

  //-----

  CFileMgrDir::file_iterator p2 = file_list.begin();
  CFileMgrDir::file_iterator p1 = p2++;
  CFileMgrDir::file_iterator pe = file_list.end();

  if (p1 == pe)
    return;

  for ( ; p2 != pe; p1 = p2, ++p2) {
  }

  (*p1)->setSelected(true);
}

void
CFileMgr::
activateSelected()
{
  CFileMgrDir::FileList selected = getSelected();

  if (selected.size() == 0)
    return;

  if (selected.size() == 1) {
    activate(*selected.begin());

    return;
  }

  CFileMgrDir::FileList::iterator pfile1 = selected.begin();
  CFileMgrDir::FileList::iterator pfile2 = selected.end  ();

  for ( ; pfile1 != pfile2; ++pfile1) {
    CFileType type = (*pfile1)->getFileType();

    if (type == CFILE_TYPE_NONE || type == CFILE_TYPE_INODE_DIR)
      return;
  }

  activateFiles(selected);
}

void
CFileMgr::
activate(int x, int y)
{
  CFileMgrFile *file = xyToFile(x, y);

  if (file != nullptr)
    activate(file);
}

void
CFileMgr::
activate(CFileMgrFile *file)
{
  if (file->isDirectory())
    activateDir(file->getPathName());
  else
    activateFile(file);
}

void
CFileMgr::
activateFile(const std::string &filename)
{
  CFile file(filename);

  CFileType type = CFileUtil::getType(&file);

  activateFileType(filename, type);
}

void
CFileMgr::
activateFile(CFileMgrFile *file)
{
  activateFileType(file->getFileName(), file->getFileType());
}

void
CFileMgr::
activateFiles(CFileMgrDir::FileList &files)
{
  CFileMgrDir::FileList::iterator pfile1 = files.begin();
  CFileMgrDir::FileList::iterator pfile2 = files.end  ();

  std::string appname, appname1;

  for ( ; pfile1 != pfile2; ++pfile1) {
    CFileType type = (*pfile1)->getFileType();

    if (! CMimeInst->getApplication(type, "multi-open", appname1))
      return;

    if (appname1 == "")
      return;

    if      (appname == "")
      appname = appname1;
    else if (appname != appname1)
      return;
  }

  if (appname == "")
    return;

  std::string filenames;

  pfile1 = files.begin();

  for ( ; pfile1 != pfile2; ++pfile1)
    filenames += " " + (*pfile1)->getFileName();

  std::string cmd = CStrUtil::replace(appname, "%p", filenames);

  CCommandMgrInst->execCommand(cmd);
}

void
CFileMgr::
activateFileType(const std::string &filename, CFileType type)
{
  std::string cmd;

  if (! CMimeInst->getOpenFileCommand(filename, type, cmd))
    return;

  CCommandMgrInst->execCommand(cmd);
}

void
CFileMgr::
activateDir(const std::string &dirname)
{
//getDir()->reload(dirname);

  setDirName(dirname);

  redraw();
}

CFileMgrDir::FileList
CFileMgr::
getSelected()
{
  return getDir()->getSelected();
}

void
CFileMgr::
selectNext()
{
  CFileMgrDir::FileList file_list;

  getFileList(file_list);

  //-----

  CFileMgrDir::file_iterator p2 = file_list.begin();
  CFileMgrDir::file_iterator p1 = p2++;
  CFileMgrDir::file_iterator pe = file_list.end();

  if (p1 == pe)
    return;

  for ( ; p2 != pe; p1 = p2, ++p2)
    if ((*p1)->getSelected())
      break;

  deselectAll();

  if (p2 != pe)
    (*p2)->setSelected(true);
  else
    selectLast();
}

void
CFileMgr::
selectPrev()
{
  CFileMgrDir::FileList file_list;

  getFileList(file_list);

  //-----

  CFileMgrDir::file_iterator p2 = file_list.begin();
  CFileMgrDir::file_iterator p1 = p2++;
  CFileMgrDir::file_iterator pe = file_list.end();

  if (p1 == pe)
    return;

  if ((*p1)->getSelected())
    return;

  for ( ; p2 != pe; p1 = p2, ++p2)
    if ((*p2)->getSelected())
      break;

  deselectAll();

  if (p2 != pe)
    (*p1)->setSelected(true);
  else
    selectFirst();
}

void
CFileMgr::
getFileList(CFileMgrDir::FileList &files)
{
  CFileMgrDir::file_iterator p1 = getDir()->fileBegin();
  CFileMgrDir::file_iterator p2 = getDir()->fileEnd  ();

  for ( ; p1 != p2; ++p1) {
    if (! (*p1)->isDirectory())
      continue;

    if (! getShowHidden()) {
      if (! (*p1)->isParentDir() && (*p1)->isHidden())
        continue;
    }

    files.push_back(*p1);
  }

  p1 = getDir()->fileBegin();

  for ( ; p1 != p2; ++p1) {
    if ((*p1)->isDirectory())
      continue;

    if (! getShowHidden() && (*p1)->isHidden())
      continue;

    files.push_back(*p1);
  }
}

CFileMgrDir::file_iterator
CFileMgr::
fileBegin()
{
  return getDir()->fileBegin();
}

CFileMgrDir::file_iterator
CFileMgr::
fileEnd()
{
  return getDir()->fileEnd();
}

CFileMgrFile *
CFileMgr::
xyToFile(int x, int y) const
{
  CFileMgrDir::file_iterator p1 = getDir()->fileBegin();
  CFileMgrDir::file_iterator p2 = getDir()->fileEnd  ();

  for ( ; p1 != p2; ++p1)
    if ((*p1)->inside(x, y))
      return *p1;

  return nullptr;
}

CImagePtr
CFileMgr::
getImage(const std::string &fileName)
{
  return CFileIconMgrInst->getImage(fileName);
}

CImagePtr
CFileMgr::
getImage(CFileType type)
{
  return CFileIconMgrInst->getImage(type);
}

void
CFileMgr::
scroll()
{
  redraw();
}

void
CFileMgr::
drawDir(CPixelRenderer *renderer, int x, int y, CFileMgrFile *file)
{
  if (! renderer)
    return;

  int icon_width  = getIconWidth ();
  int icon_height = getIconHeight();

  int x1 = x;
  int y1 = y;
  int x2 = x + icon_width  - 1;
  int y2 = y + icon_height - 1;

  bool visible = (x2 >= 0 && x1 < int(renderer->getWidth ()) &&
                  y2 >= 0 && y1 < int(renderer->getHeight()));

  CIPoint2D p1(x1, y1);
  CIPoint2D p2(x2, y2);

  renderer->unapplyOffset(p1);
  renderer->unapplyOffset(p2);

  file->setBBox(p1.x, p1.y, p2.x, p2.y);

  if (! visible)
    return;

  CImagePtr image = getImage(CFILE_TYPE_INODE_DIR);

  if (! image.isValid())
    return;

  drawFileImage(renderer, x, y, image);

  drawFileText(renderer, x, y, file->getFileName());

  drawFileBorder(renderer, x, y, file->getSelected());
}

void
CFileMgr::
drawFile(CPixelRenderer *renderer, int x, int y, CFileMgrFile *file)
{
  if (! renderer)
    return;

  int icon_width  = getIconWidth ();
  int icon_height = getIconHeight();

  int x1 = x;
  int y1 = y;
  int x2 = x + icon_width  - 1;
  int y2 = y + icon_height - 1;

  bool visible = (x2 >= 0 && x1 < int(renderer->getWidth ()) &&
                  y2 >= 0 && y1 < int(renderer->getHeight()));

  CIPoint2D p1(x1, y1);
  CIPoint2D p2(x2, y2);

  renderer->unapplyOffset(p1);
  renderer->unapplyOffset(p2);

  file->setBBox(p1.x, p1.y, p2.x, p2.y);

  if (! visible)
    return;

  CImagePtr image = getImage(file->getPathName());

  drawFileImage(renderer, x, y, image);

  drawFileText(renderer, x, y, file->getFileName());

  drawFileBorder(renderer, x, y, file->getSelected());
}

void
CFileMgr::
drawFileImage(CPixelRenderer *renderer, int x, int y, const CImagePtr &image)
{
  if (! renderer)
    return;

  if (! image)
    return;

  int icon_width   = getIconWidth ();
  int image_height = getImageHeight();
  int border       = getBorder     ();

  int dx = (icon_width   - image->getWidth ())/2;
  int dy = (image_height - image->getHeight())/2;

  CIPoint2D p(x, y);

  renderer->unapplyOffset(p);

  renderer->drawAlphaImage(CIPoint2D(p.x + dx, p.y + 2*border + dy), image);
}

void
CFileMgr::
drawFileText(CPixelRenderer *renderer, int x, int y, const std::string &text)
{
  if (! renderer)
    return;

  int icon_width  = getIconWidth ();
  int icon_height = getIconHeight();
  int border      = getBorder    ();

  std::string text1 = text;

  int text_width = renderer->getStringWidth(text1);

  while (text_width > icon_width) {
    text1 = text1.substr(0, text1.size() - 1);

    text_width = renderer->getStringWidth(text1);
  }

  int text_height = renderer->getCharHeight();

  int dx = (icon_width - text_width)/2;

  renderer->setForeground(CRGB(0));

  CIPoint2D p(x, y);

  renderer->unapplyOffset(p);

  renderer->drawString(CIPoint2D(p.x + dx, p.y + icon_height - text_height - border), text1);
}

void
CFileMgr::
drawFileBorder(CPixelRenderer *renderer, int x, int y, bool selected)
{
  if (! renderer)
    return;

  if (selected)
    renderer->setForeground(CRGB(0,0,0.7));
  else
    renderer->setForeground(CRGB(0.5));

  int icon_width    = getIconWidth ();
  int image_width   = getImageWidth ();
  int image_height  = getImageHeight();
  int border        = getBorder     ();
  int border_width  = image_width  + 2*border;
  int border_height = image_height + 2*border;

  int dx = (icon_width - border_width)/2;

  CIPoint2D p(x, y);

  renderer->unapplyOffset(p);

  int x1 = p.x + dx;
  int x2 = x1 + border_width;
  int y1 = p.y + border;
  int y2 = y1 + border_height;

  renderer->drawRectangle(CIBBox2D(x1, y1, x2, y2));
}
