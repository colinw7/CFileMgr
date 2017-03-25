#ifndef CQFILEMGR_TOOLBAR_H
#define CQFILEMGR_TOOLBAR_H

#include <QWidget>

class CQFileMgr;
class CQImageButton;

class CQFileMgrToolbar : public QWidget {
  Q_OBJECT

 public:
  enum IconType {
    ADD_PLACE_BUTTON
  };

 public:
  CQFileMgrToolbar(CQFileMgr *mgr);
 ~CQFileMgrToolbar();

  void showIcon(IconType type, bool show);

 private slots:
  void updateState();

 private:
  void init();

 private:
  CQFileMgr     *mgr_ { nullptr };
  CQImageButton *backButton_ { nullptr };
  CQImageButton *forwardButton_ { nullptr };
  CQImageButton *iconsButton_ { nullptr };
  CQImageButton *detailsButton_ { nullptr };
  CQImageButton *stripButton_ { nullptr };
  CQImageButton *imageButton_ { nullptr };
  CQImageButton *hiddenButton_ { nullptr };
  CQImageButton *refreshButton_ { nullptr };
  CQImageButton *addButton_ { nullptr };
};

#endif
