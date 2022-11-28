#ifndef CQFILEMGR_TOOLBAR_H
#define CQFILEMGR_TOOLBAR_H

#include <QWidget>

class CQFileMgr;
class CQIconButton;

class CQFileMgrToolbar : public QWidget {
  Q_OBJECT

  Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)

 public:
  enum IconType {
    ADD_PLACE_BUTTON
  };

 public:
  CQFileMgrToolbar(CQFileMgr *mgr);
 ~CQFileMgrToolbar();

  QSize iconSize() const;
  void setIconSize(const QSize &s);

  void showIcon(IconType type, bool show);

  QSize sizeHint() const override;

 private Q_SLOTS:
  void updateIconSize();

  void updateState();

 private:
  void init();

 private:
  CQFileMgr *mgr_ { nullptr };

  CQIconButton *backButton_    { nullptr };
  CQIconButton *forwardButton_ { nullptr };
  CQIconButton *iconsButton_   { nullptr };
  CQIconButton *detailsButton_ { nullptr };
  CQIconButton *stripButton_   { nullptr };
  CQIconButton *imageButton_   { nullptr };
  CQIconButton *hiddenButton_  { nullptr };
  CQIconButton *refreshButton_ { nullptr };
  CQIconButton *addButton_     { nullptr };

  using Buttons = std::vector<CQIconButton *>;

  Buttons buttons_;

  QSize iconSize_;
};

#endif
