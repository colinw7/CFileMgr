#include <CQFileMgr.h>
#include <CQDirBar.h>
#include <CQDirPlaces.h>

#include <CQFileMgrIcons.h>
#include <CQFileMgrFilmstripIcons.h>
#include <CQFileMgrFilmstripImage.h>
#include <CQFileMgrDetails.h>
#include <CQFileMgrToolbar.h>
#include <CQFileMgrInfo.h>

#include <CQUtil.h>
#include <CQDirView.h>
#include <CFileMgrLib.h>
#include <CDir.h>
#include <COSFile.h>

#include <QAction>
#include <QMenu>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QLabel>

class CFileMgrImpl : public CFileMgr {
 public:
  CFileMgrImpl(CQFileMgr *filemgr) :
   CFileMgr(), filemgr_(filemgr) {
  }

  virtual void redraw() {
    filemgr_->changed();
  }

  virtual void changeDir(const std::string &dirname) {
    filemgr_->changeDir(dirname);
  }

  virtual void setTableColumns(const CFileMgrDetails::ColumnList &columns) {
    filemgr_->setTableColumns(columns);
  }

  virtual void deleteAllTableRows() {
    filemgr_->deleteAllTableRows();
  }

  virtual void addTableRow(CFileMgrFile *file, const std::vector<std::string> &values) {
    filemgr_->addTableRow(file, values);
  }

  virtual void activateFileType(const std::string &filename, CFileType type) {
    if (! filemgr_->activateFileTypeFwd(filename, type))
      CFileMgr::activateFileType(filename, type);
  }

 private:
  CQFileMgr *filemgr_ { nullptr };
};

//------

CQFileMgr::
CQFileMgr(QWidget *parent) :
 QWidget(parent)
{
  setObjectName("fileMgr");

  filemgr_ = new CFileMgrImpl(this);

  filemgr_->setForceRGB();

  //------

  auto *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  //------

  toolbar_ = new CQFileMgrToolbar(this);

  layout->addWidget(toolbar_);

  //------

  dirbar_ = new CQDirBar;

  layout->addWidget(dirbar_);

  connect(CQDirViewInst, SIGNAL(dirChangedSignal(const QString &)),
          this, SLOT(setDir(const QString &)));

  //------

  auto *splitter = new QSplitter;
  splitter->setObjectName("splitter");

  layout->addWidget(splitter);

  //------

  places_frame_ = new QWidget;
  places_frame_->setObjectName("places");

  auto *places_layout = new QVBoxLayout(places_frame_);
  places_layout->setMargin(2); places_layout->setSpacing(2);

  auto *label = new QLabel("Places");

  places_layout->addWidget(label);

  places_ = new CQDirPlaces(this);

  places_layout->addWidget(places_);

  splitter->addWidget(places_frame_);

  //------

  auto *browser = new QWidget;
  browser->setObjectName("browser");

  browser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  auto *vlayout = new QVBoxLayout(browser);
  vlayout->setMargin(0); vlayout->setSpacing(0);

  //------

  // Icons Frame
  icons_frame_ = new QWidget(this);

  icons_frame_->setObjectName("icons_frame");

  auto *vlayout1 = new QVBoxLayout(icons_frame_);
  vlayout1->setMargin(0); vlayout1->setSpacing(0);

  icons_canvas_ = new CQFileMgrIcons(icons_frame_, this);

  vlayout1->addWidget(icons_canvas_);

  //------

  // Filmstrip Frame
  filmstrip_frame_ = new QWidget(this);

  filmstrip_frame_->setObjectName("filmstrip_frame");

  auto *vlayout2 = new QVBoxLayout(filmstrip_frame_);
  vlayout2->setMargin(0); vlayout2->setSpacing(0);

  filmstrip_image_canvas_ = new CQFileMgrFilmstripImage(filmstrip_frame_, this);
  filmstrip_icons_canvas_ = new CQFileMgrFilmstripIcons(filmstrip_frame_, this);

  vlayout2->addWidget(filmstrip_image_canvas_);
  vlayout2->addWidget(filmstrip_icons_canvas_);

  filmstrip_frame_->hide();

  //------

  // Details Frame

  details_frame_ = new QWidget(this);

  details_frame_->setObjectName("details_frame");

  auto *vlayout3 = new QVBoxLayout(details_frame_);
  vlayout3->setMargin(0); vlayout3->setSpacing(0);

  details_table_ = new CQFileMgrDetails(details_frame_, this);

  vlayout3->addWidget(details_table_);

  details_frame_->hide();

  //------

  vlayout->addWidget(icons_frame_);
  vlayout->addWidget(filmstrip_frame_);
  vlayout->addWidget(details_frame_);

  //------

  splitter->addWidget(browser);

  QFontMetrics fm(font());

  int pw = fm.horizontalAdvance("/home/user") + 32;

  places_frame_->setFixedWidth(pw);

  //------

  filemgr_->setDirName(CDir::getCurrent());

  //------

  updateFilmstripSize();

  changed();
}

CQFileMgr::
~CQFileMgr()
{
  delete filemgr_;
  delete icons_canvas_;
  delete filmstrip_image_canvas_;
  delete filmstrip_icons_canvas_;
}

const std::string &
CQFileMgr::
getDirName() const
{
  return filemgr_->getDirName();
}

void
CQFileMgr::
setDirName(const std::string &dirName)
{
  filemgr_->setDirName(dirName);
}

int
CQFileMgr::
getSmallIconSize() const
{
  return filemgr_->getSmallIconSize();
}

void
CQFileMgr::
setSmallIconSize(int s)
{
  filemgr_->setSmallIconSize(s);

  changed();
}

int
CQFileMgr::
getLargeIconSize() const
{
  return filemgr_->getLargeIconSize();
}

void
CQFileMgr::
setLargeIconSize(int s)
{
  filemgr_->setLargeIconSize(s);

  updateFilmstripSize();

  changed();
}

void
CQFileMgr::
showPlaces(bool show)
{
  places_frame_->setVisible(show);

  toolbar_->showIcon(CQFileMgrToolbar::ADD_PLACE_BUTTON, show);
}

void
CQFileMgr::
resize()
{
}

void
CQFileMgr::
changed()
{
  if      (filemgr_->getViewType() == CFileMgr::VIEW_ICONS) {
    if (icons_canvas_)
      icons_canvas_->setContentsChanged();
  }
  else if (filemgr_->getViewType() == CFileMgr::VIEW_FILMSTRIP) {
    if (filmstrip_image_canvas_)
      filmstrip_image_canvas_->setContentsChanged();

    if (filmstrip_icons_canvas_)
      filmstrip_icons_canvas_->setContentsChanged();
  }

  expose();
}

void
CQFileMgr::
expose()
{
  if      (filemgr_->getViewType() == CFileMgr::VIEW_ICONS) {
    if (icons_canvas_)
      icons_canvas_->update();
  }
  else if (filemgr_->getViewType() == CFileMgr::VIEW_FILMSTRIP) {
    if (filmstrip_image_canvas_)
      filmstrip_image_canvas_->update();

    if (filmstrip_icons_canvas_)
      filmstrip_icons_canvas_->update();
  }
}

void
CQFileMgr::
keyPress(const CKeyEvent &event)
{
  filemgr_->keyPress(event);
}

void
CQFileMgr::
keyRelease(const CKeyEvent &event)
{
  filemgr_->keyRelease(event);
}

void
CQFileMgr::
draw()
{
  filemgr_->draw();
}

void
CQFileMgr::
drawIcons()
{
  filemgr_->drawIcons();
}

void
CQFileMgr::
drawFilmstripImage()
{
  filemgr_->drawFilmstripImage();
}

void
CQFileMgr::
drawFilmstripIcons()
{
  filemgr_->drawFilmstripIcons();
}

int
CQFileMgr::
getCanvasWidth() const
{
  if      (filemgr_->getViewType() == CFileMgr::VIEW_ICONS)
    return icons_canvas_->width();
  else if (filemgr_->getViewType() == CFileMgr::VIEW_FILMSTRIP)
    return filmstrip_icons_canvas_->width();
  else
    return details_table_->width();
}

int
CQFileMgr::
getCanvasHeight() const
{
  if      (filemgr_->getViewType() == CFileMgr::VIEW_ICONS)
    return icons_canvas_->height();
  else if (filemgr_->getViewType() == CFileMgr::VIEW_FILMSTRIP)
    return filmstrip_icons_canvas_->height();
  else
    return details_table_->height();
}

int
CQFileMgr::
getContentsWidth() const
{
  if (filemgr_->getViewType() == CFileMgr::VIEW_FILMSTRIP) {
    auto *filmstrip = filemgr_->getFilmstrip();

    if (filmstrip)
      return filmstrip->getWidth();
  }

  return getCanvasWidth();
}

int
CQFileMgr::
getContentsHeight() const
{
  if (filemgr_->getViewType() == CFileMgr::VIEW_ICONS) {
    auto *icons = filemgr_->getIcons();

    if (icons)
      return icons->getHeight();
  }

  return getCanvasHeight();
}

void
CQFileMgr::
setIconsView()
{
  icons_frame_    ->show();
  filmstrip_frame_->hide();
  details_frame_  ->hide();

  filemgr_->setIconsView();

  expose();
}

void
CQFileMgr::
setFilmstripView()
{
  icons_frame_    ->hide();
  filmstrip_frame_->show();
  details_frame_  ->hide();

  filemgr_->setFilmstripView();

  expose();
}

void
CQFileMgr::
setDetailsView()
{
  icons_frame_    ->hide();
  filmstrip_frame_->hide();
  details_frame_  ->show();

  filemgr_->setDetailsView();

  expose();
}

bool
CQFileMgr::
getSmallIcons()
{
  return filemgr_->getSmallIcons();
}

bool
CQFileMgr::
getShowImages()
{
  return filemgr_->getShowImages();
}

bool
CQFileMgr::
getShowHidden()
{
  return filemgr_->getShowHidden();
}

void
CQFileMgr::
setShowImages(bool show_images)
{
  filemgr_->setShowImages(show_images);

  changed();

  emit stateChanged();
}

void
CQFileMgr::
setSmallIcons(bool small_icons)
{
  filemgr_->setSmallIcons(small_icons);

  changed();

  emit stateChanged();
}

void
CQFileMgr::
setShowHidden(bool show_hidden)
{
  filemgr_->setShowHidden(show_hidden);

  changed();

  emit stateChanged();
}

void
CQFileMgr::
deselectAll()
{
  filemgr_->deselectAll();
}

void
CQFileMgr::
selectInside(const CIBBox2D &bbox)
{
  if (filemgr_->getViewType() == CFileMgr::VIEW_ICONS ||
      filemgr_->getViewType() == CFileMgr::VIEW_FILMSTRIP) {
    CFileMgrDir::file_iterator p1 = filemgr_->fileBegin();
    CFileMgrDir::file_iterator p2 = filemgr_->fileEnd  ();

    deselectAll();

    for ( ; p1 != p2; ++p1) {
      if ((*p1)->inside(bbox))
        (*p1)->setSelected(true);
    }
  }

  changed();
}

void
CQFileMgr::
activate(const CIPoint2D &position)
{
  if (filemgr_->getViewType() == CFileMgr::VIEW_ICONS ||
      filemgr_->getViewType() == CFileMgr::VIEW_FILMSTRIP) {
    deselectAll();

    CFileMgrDir::file_iterator p1 = filemgr_->fileBegin();
    CFileMgrDir::file_iterator p2 = filemgr_->fileEnd  ();

    for ( ; p1 != p2; ++p1) {
      if (! (*p1)->inside(position))
        continue;

      activate(*p1);

      break;
    }
  }
}

void
CQFileMgr::
activate(CFileMgrFile *file)
{
  if (file->isDirectory())
    activateDir(file->getPathName());
  else
    activateFile(file->getFileName());

  changed();
}

void
CQFileMgr::
activateFile(const std::string &filename)
{
  filemgr_->activateFile(filename);

  emit fileActivated(filename.c_str());
}

void
CQFileMgr::
activateDir(const std::string &dirname)
{
  filemgr_->activateDir(dirname);

  emit dirActivated(dirname.c_str());
}

bool
CQFileMgr::
activateFileTypeFwd(const std::string &filename, CFileType type)
{
  activated_ = true;

  activateFileType(filename, type);

  return activated_;
}

void
CQFileMgr::
activateFileType(const std::string &, CFileType)
{
  activated_ = false;
}

void
CQFileMgr::
changeDir(const std::string &dirname)
{
  CDirViewInst->setDirName(dirname.c_str());

  emit dirChanged(dirname.c_str());
}

void
CQFileMgr::
refresh()
{
  activateDir(COSFile::getCurrentDir());
}

//----------

void
CQFileMgr::
setTableColumns(const CFileMgrDetails::ColumnList &columns)
{
  auto *table = getDetailsTable();

  table->setColumns(columns);
}

void
CQFileMgr::
deleteAllTableRows()
{
  auto *table = getDetailsTable();

  table->clear();
}

void
CQFileMgr::
addTableRow(CFileMgrFile *file, const std::vector<std::string> &values)
{
  auto *table = getDetailsTable();

  table->addRow(file, values);
}

//----------

void
CQFileMgr::
popupMenu(QWidget *parent, int x, int y)
{
  static QAction *iconsAction;
  static QAction *filmstripAction;
  static QAction *detailsAction;
  static QAction *smallIconsAction;
  static QAction *showImagesAction;
  static QAction *showHiddenAction;
  static QAction *infoAction;

  if (! showImagesAction) {
    iconsAction      = new QAction("&Icons View"    , this);
    filmstripAction  = new QAction("&Filmstrip View", this);
    detailsAction    = new QAction("&Details View"  , this);
    smallIconsAction = new QAction("Small I&cons"   , this);
    showImagesAction = new QAction("Show &Images"   , this);
    showHiddenAction = new QAction("Show &Hidden"   , this);
    infoAction       = new QAction("Info"           , this);

    smallIconsAction->setCheckable(true);
    showImagesAction->setCheckable(true);
    showHiddenAction->setCheckable(true);

    connect(iconsAction     , SIGNAL(triggered())  , this, SLOT(iconsView()));
    connect(filmstripAction , SIGNAL(triggered())  , this, SLOT(filmstripView()));
    connect(detailsAction   , SIGNAL(triggered())  , this, SLOT(detailsView()));
    connect(showImagesAction, SIGNAL(toggled(bool)), this, SLOT(setShowImages(bool)));
    connect(smallIconsAction, SIGNAL(toggled(bool)), this, SLOT(setSmallIcons(bool)));
    connect(showHiddenAction, SIGNAL(toggled(bool)), this, SLOT(setShowHidden(bool)));
    connect(infoAction      , SIGNAL(triggered())  , this, SLOT(showInfo()));
  }

  QMenu menu(parent);

  smallIconsAction->setChecked(filemgr_->getSmallIcons());
  showImagesAction->setChecked(filemgr_->getShowImages());
  showHiddenAction->setChecked(filemgr_->getShowHidden());

  if      (filemgr_->getViewType() == CFileMgr::VIEW_ICONS) {
    menu.addAction(filmstripAction);
    menu.addAction(detailsAction);

    menu.addAction(smallIconsAction);
    menu.addAction(showImagesAction);
    menu.addAction(showHiddenAction);
  }
  else if (filemgr_->getViewType() == CFileMgr::VIEW_FILMSTRIP) {
    menu.addAction(iconsAction);
    menu.addAction(detailsAction);

    menu.addAction(smallIconsAction);
    menu.addAction(showImagesAction);
    menu.addAction(showHiddenAction);
  }

  menu.addAction(infoAction);

  QPoint point(x, y);

  menu.exec(point);
}

void
CQFileMgr::
undoDir()
{
  filemgr_->undoDir();
}

void
CQFileMgr::
redoDir()
{
  filemgr_->redoDir();
}

void
CQFileMgr::
parentDir()
{
  CDir dir(getDirName());

  setDir(dir.getParentName().c_str());
}

void
CQFileMgr::
setDir(const QString &dirName)
{
  activateDir(dirName.toStdString());

  changed();
}

void
CQFileMgr::
iconsView()
{
  setIconsView();
}

void
CQFileMgr::
filmstripView()
{
  setFilmstripView();
}

void
CQFileMgr::
detailsView()
{
  setDetailsView();
}

void
CQFileMgr::
addToPlaces()
{
  places_->addPlace(CDir::getCurrent(), CDir::getCurrent());
}

void
CQFileMgr::
showInfo()
{
  if (! info_)
    info_ = new CQFileMgrInfo(this);

  CFileMgrDir::FileList selected = filemgr_->getSelected();

  CFileMgrFile *file = nullptr;

  if (! selected.empty())
    file = *selected.begin();

  if (file)
    info_->init(file->getPathName());
  else
    info_->init(CDir::getCurrent());

  info_->show();
}

void
CQFileMgr::
updateFilmstripSize()
{
  int is = getLargeIconSize();
  int fh = QFontMetrics(font()).height();

  CISize2D size = filemgr_->getSizeForIcon(is, fh);

  int scrollBarExtent = style()->pixelMetric(QStyle::PM_ScrollBarExtent);

  filmstrip_icons_canvas_->setFixedHeight(size.height + scrollBarExtent);
}
