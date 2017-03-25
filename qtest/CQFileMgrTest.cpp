#include <CQFileMgrTest.h>
#include <CDir.h>
#include <CFileUtil.h>
#include <CQFileMgr.h>
#ifdef TERM_WIDGET
#include <CQTerm.h>
#endif
#include <CQImageCanvas.h>
//#include <CwshLib.h>
//#include <CMessage.h>

#include <QVBoxLayout>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QLabel>
#include <QSplitter>
#include <QStatusBar>
#include <QTimer>

#include <sys/param.h>

#include <svg/back_svg.h>
#include <svg/forward_svg.h>
#include <svg/up_svg.h>
#include <svg/icons_svg.h>
#include <svg/strip_svg.h>
#include <svg/details_svg.h>
#include <svg/image_svg.h>
#include <svg/hidden_svg.h>
#include <svg/add_svg.h>

CQFileMgrTest::
CQFileMgrTest() :
 filemgr_(0), term_(0), image_canvas_(0)
{
  resize(900, 600);
}

QWidget *
CQFileMgrTest::
createCentralWidget()
{
  QSplitter *splitter = new QSplitter(Qt::Vertical);
  splitter->setObjectName("splitter");

  //-----

  QWidget *mgr_frame = new QWidget;
  mgr_frame->setObjectName("mgr_frame");

  QVBoxLayout *mgr_layout = new QVBoxLayout(mgr_frame);
  mgr_layout->setMargin(0); mgr_layout->setSpacing(0);

  filemgr_ = new CQFileMgr;

  filemgr_->getFileMgr()->setShowDotDot(false);

  connect(filemgr_, SIGNAL(dirChanged(const QString &)),
          this, SLOT(dirChanged(const QString &)));

  connect(filemgr_, SIGNAL(fileActivated(const QString &)),
          this, SLOT(fileActivated(const QString &)));

  connect(filemgr_, SIGNAL(stateChanged()), this, SLOT(updateState()));

  mgr_layout->addWidget(filemgr_);

  splitter->addWidget(mgr_frame);

  //-----

#ifdef TERM_WIDGET
  QWidget *term_frame = new QWidget;
  term_frame->setObjectName("term_frame");

  QVBoxLayout *term_layout = new QVBoxLayout(term_frame);
  term_layout->setMargin(0); term_layout->setSpacing(0);

  term_ = new CQTerm;

  term_->init();

  term_layout->addWidget(term_);

  splitter->addWidget(term_frame);
#endif

  //-----

  setWindowTitle(CDir::getCurrent().c_str());

  //-----

  //timer_ = new QTimer(this);

  //connect(timer_, SIGNAL(timeout()), this, SLOT(timerSlot()));

  //timer_->start(250);

  //-----

  return splitter;
}

void
CQFileMgrTest::
createActions()
{
  exitAction_ = new QAction("E&xit", this);

  exitAction_->setShortcut(QKeySequence("Ctrl+Q"));
  exitAction_->setStatusTip("Exit the application");

  connect(exitAction_, SIGNAL(triggered()), this, SLOT(close()));

  //------

  backAction_ = new QAction("&Previous Directory", this);

  backAction_->setIcon(CQPixmapCacheInst->getIcon("BACK"));

  backAction_->setStatusTip("Previous Directory");

  connect(backAction_, SIGNAL(triggered()), filemgr_, SLOT(undoDir()));

  forwardAction_ = new QAction("&Undo Previous Directory", this);

  forwardAction_->setIcon(CQPixmapCacheInst->getIcon("FORWARD"));

  forwardAction_->setStatusTip("Undo Previous Directory");

  connect(forwardAction_, SIGNAL(triggered()), filemgr_, SLOT(redoDir()));

  //------

  upAction_ = new QAction("&Parent Directory", this);

  upAction_->setIcon(CQPixmapCacheInst->getIcon("UP"));

  upAction_->setStatusTip("Parent Directory");

  connect(upAction_, SIGNAL(triggered()), filemgr_, SLOT(parentDir()));

  //------

  iconsAction_ = new QAction("&Icons View", this);

  iconsAction_->setIcon(CQPixmapCacheInst->getIcon("ICONS"));

//iconsAction_->setCheckable(true);
//iconsAction_->setChecked(true);

  iconsAction_->setStatusTip("Icons View");

  connect(iconsAction_, SIGNAL(triggered()), filemgr_, SLOT(iconsView()));

  //------

  filmstripAction_ = new QAction("&Filmstrip View", this);

  filmstripAction_->setIcon(CQPixmapCacheInst->getIcon("STRIP"));

//filmstripAction_->setCheckable(true);
//filmstripAction_->setChecked(true);

  filmstripAction_->setStatusTip("Filmstrip View");

  connect(filmstripAction_, SIGNAL(triggered()), filemgr_, SLOT(filmstripView()));

  //------

  detailsAction_ = new QAction("&Details View", this);

  detailsAction_->setIcon(CQPixmapCacheInst->getIcon("DETAILS"));

//detailsAction_->setCheckable(true);
//detailsAction_->setChecked(true);

  detailsAction_->setStatusTip("Details View");

  connect(detailsAction_, SIGNAL(triggered()), filemgr_, SLOT(detailsView()));

  //------

  imageAction_ = new QAction("&Display Images", this);

  imageAction_->setCheckable(true);
  imageAction_->setIcon(CQPixmapCacheInst->getIcon("IMAGE"));
  imageAction_->setChecked(filemgr_->getShowImages());

  imageAction_->setStatusTip("Display Images");

  connect(imageAction_, SIGNAL(triggered(bool)), filemgr_, SLOT(setShowImages(bool)));

  //------

  hiddenAction_ = new QAction("&Display Hidden", this);

  hiddenAction_->setIcon(CQPixmapCacheInst->getIcon("HIDDEN"));

  hiddenAction_->setStatusTip("Display Hidden");

  connect(hiddenAction_, SIGNAL(triggered(bool)), filemgr_, SLOT(setShowHidden(bool)));

  //------

  addAction_ = new QAction("&Add to Places", this);

  addAction_->setIcon(CQPixmapCacheInst->getIcon("ADD"));

  addAction_->setStatusTip("Display Hidden");

  connect(addAction_, SIGNAL(triggered()), filemgr_, SLOT(addToPlaces()));

  //------

  helpAction_ = new QAction("&Help", this);
}

void
CQFileMgrTest::
createMenus()
{
  createActions();

  fileMenu_ = menuBar()->addMenu("&File");

  fileMenu_->addAction(exitAction_);

  viewMenu_ = menuBar()->addMenu("&View");

  viewMenu_->addAction(backAction_);
  viewMenu_->addAction(forwardAction_);
  viewMenu_->addAction(upAction_);
  viewMenu_->addAction(iconsAction_);
  viewMenu_->addAction(filmstripAction_);
  viewMenu_->addAction(detailsAction_);
  viewMenu_->addAction(imageAction_);
  viewMenu_->addAction(hiddenAction_);
  viewMenu_->addAction(addAction_);

  helpMenu_ = menuBar()->addMenu("&Help");

  helpMenu_->addAction(helpAction_);
}

void
CQFileMgrTest::
createToolBars()
{
#if 0
  viewToolBar_ = addToolBar("&View");

  viewToolBar_->addAction(backAction_);
  viewToolBar_->addAction(forwardAction_);
  viewToolBar_->addAction(upAction_);
  viewToolBar_->addAction(iconsAction_);
  viewToolBar_->addAction(filmstripAction_);
  viewToolBar_->addAction(detailsAction_);
  viewToolBar_->addAction(imageAction_);
  viewToolBar_->addAction(hiddenAction_);
  viewToolBar_->addAction(addAction_);
#endif
}

void
CQFileMgrTest::
createStatusBar()
{
  statusLabel_ = new QLabel(" ");

  statusLabel_->setObjectName("label");

  statusLabel_->setAlignment(Qt::AlignHCenter);
  statusLabel_->setMinimumSize(statusLabel_->sizeHint());

  statusBar()->setObjectName("statusBar");

  statusBar()->addWidget(statusLabel_);
}

void
CQFileMgrTest::
dirChanged(const QString &dirname)
{
  setWindowTitle(dirname);

  //if (! cwsh_msg_) cwsh_msg_ = Cwsh::createServerMessage();

  //std::string reply;

  //cwsh_msg_->sendClientMessageAndRecv("cd " + dirname.toStdString(), reply);
}

void
CQFileMgrTest::
fileActivated(const QString &filename)
{
  std::string name = filename.toStdString();

  CFile file(name);

  CFileType type = CFileUtil::getType(&file);

  if (type & CFILE_TYPE_IMAGE) {
    if (! image_canvas_)
      image_canvas_ = new CQImageCanvas;

    image_canvas_->updateImage(name);

    ((QWidget *) image_canvas_)->resize(image_canvas_->getImageWidth(),
                                        image_canvas_->getImageHeight());

    image_canvas_->show();
  }
  else
    std::cerr << name << std::endl;
}

void
CQFileMgrTest::
timerSlot()
{
  static char last_dir[MAXPATHLEN + 1];

  //if (! cwsh_mem_) cwsh_mem_ = new CwshShMem;

  char dir[MAXPATHLEN + 1];

  //if (! cwsh_mem_->getPath(dir)) return;

  if (strcmp(dir, last_dir) != 0) {
    strcpy(last_dir, dir);

    filemgr_->setDir(dir);
  }

#if 0
  static std::string last_dir;

  if (! cwsh_msg_) cwsh_msg_ = Cwsh::createServerMessage();

  std::string dir;

  cwsh_msg_->sendClientMessageAndRecv("pwd", dir);

  if (dir != last_dir) {
    last_dir = dir;

    filemgr_->setDir(dir.c_str());
  }
#endif
}

void
CQFileMgrTest::
updateState()
{
  iconsAction_ ->setChecked(filemgr_->getSmallIcons());
  imageAction_ ->setChecked(filemgr_->getShowImages());
  hiddenAction_->setChecked(filemgr_->getShowHidden());
}
