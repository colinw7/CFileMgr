#include <CQMainWindow.h>
#include <CAutoPtr.h>
//#include <CMessage.h>
//#include <CwshShMem.h>

class CQFileMgr;
class CQTerm;
class CQImageCanvas;
class QAction;
class QMenu;
class QToolBar;
class QLabel;
class QTimer;
class CwshShMem;
//class CMessage;

class CQFileMgrTest : public CQMainWindow {
  Q_OBJECT

 public:
  CQFileMgrTest();

 private:
  QWidget *createCentralWidget();

  void createActions();
  void createMenus();
  void createToolBars();
  void createStatusBar();

 private slots:
  void dirChanged(const QString &dirname);
  void fileActivated(const QString &dirname);
  void timerSlot();
  void updateState();

 private:
  CQFileMgr     *filemgr_ { nullptr };
  CQTerm        *term_ { nullptr };
  CQImageCanvas *image_canvas_ { nullptr };

  QAction   *exitAction_ { nullptr };
  QAction   *backAction_ { nullptr };
  QAction   *forwardAction_ { nullptr };
  QAction   *upAction_ { nullptr };
  QAction   *iconsAction_ { nullptr };
  QAction   *filmstripAction_ { nullptr };
  QAction   *detailsAction_ { nullptr };
  QAction   *imageAction_ { nullptr };
  QAction   *hiddenAction_ { nullptr };
  QAction   *addAction_ { nullptr };
  QAction   *helpAction_ { nullptr };
  QMenu     *fileMenu_ { nullptr };
  QMenu     *viewMenu_ { nullptr };
  QMenu     *helpMenu_ { nullptr };
  QToolBar  *viewToolBar_ { nullptr };
  QLabel    *statusLabel_ { nullptr };

  QTimer    *timer_ { nullptr };

//CAutoPtr<CwshShMem> cwsh_mem_;
//CAutoPtr<CMessage>  cwsh_msg_;
};
