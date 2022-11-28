#include <CQFileMgrToolbar.h>

#include <CQFileMgr.h>
#include <CQIconButton.h>
#include <CQStyleMgr.h>

#include <CConfig.h>
#include <CDir.h>
#include <CStrUtil.h>

#include <QLineEdit>
#include <QMouseEvent>
#include <QMenu>
#include <QHBoxLayout>

#include <svg/back_svg.h>
#include <svg/details_svg.h>
#include <svg/forward_svg.h>
#include <svg/hidden_svg.h>
#include <svg/icons_svg.h>
#include <svg/image_svg.h>
#include <svg/refresh_svg.h>
#include <svg/strip_svg.h>
#include <svg/favorites_svg.h>
//#include <svg/up_svg.h>

CQFileMgrToolbar::
CQFileMgrToolbar(CQFileMgr *mgr) :
 QWidget(nullptr), mgr_(mgr)
{
  setObjectName("fileMgrToolbar");

  init();

  connect(mgr_, SIGNAL(stateChanged()), this, SLOT(updateState()));

  connect(CQStyleMgrInst, SIGNAL(iconSizeChanged()), this, SLOT(updateIconSize()));
}

CQFileMgrToolbar::
~CQFileMgrToolbar()
{
}

void
CQFileMgrToolbar::
init()
{
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  auto createButton = [&](const QString &iconName, const QString &tooltip,
                          const char *slotName=nullptr) {
    auto *button = new CQIconButton(iconName);
    button->setToolTip(tooltip);
    if (slotName)
      connect(button, SIGNAL(clicked()), mgr_, slotName);
    buttons_.push_back(button);
    return button;
  };

  backButton_    = createButton("BACK"   , "Previous Dir", SLOT(undoDir()));
  forwardButton_ = createButton("FORWARD", "Next Dir"    , SLOT(redoDir()));

  //auto *up_button = createButton(QPixmap((const char **) up_data));
  //connect(up_button, SIGNAL(clicked()), mgr_, SLOT(parentDir()));

  iconsButton_   = createButton("ICONS"  , "Icons View"  , SLOT(iconsView()));
  detailsButton_ = createButton("DETAILS", "Details View", SLOT(detailsView()));
  stripButton_   = createButton("STRIP"  , "Strip View"  , SLOT(filmstripView()));

  imageButton_  = createButton("IMAGE", "Show Images");
  imageButton_->setCheckable(true);
  connect(imageButton_, SIGNAL(clicked(bool)), mgr_, SLOT(setShowImages(bool)));

  hiddenButton_ = createButton("HIDDEN", "Show Hidden");
  hiddenButton_->setCheckable(true);
  connect(hiddenButton_, SIGNAL(clicked(bool)), mgr_, SLOT(setShowHidden(bool)));

  refreshButton_ = createButton("REFRESH"  , "Refresh View"    , SLOT(refresh()));
  addButton_     = createButton("FAVORITES", "Add to Favorties", SLOT(addToPlaces()));

  layout->addWidget(backButton_);
  layout->addWidget(forwardButton_);

  layout->addSpacing(8);

  //layout->addWidget(up_button);

  //layout->addSpacing(8);

  layout->addWidget(iconsButton_);
  layout->addWidget(detailsButton_);
  layout->addWidget(stripButton_);

  layout->addSpacing(8);

  layout->addWidget(imageButton_);
  layout->addWidget(hiddenButton_);

  layout->addSpacing(8);

  layout->addWidget(refreshButton_);

  layout->addSpacing(8);

  layout->addWidget(addButton_);

  layout->addStretch();

  //---

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

QSize
CQFileMgrToolbar::
iconSize() const
{
  if (! iconSize_.isValid()) {
    int is = CQStyleMgrInst->iconSize();

    return QSize(is, is);
  }

  return iconSize_;
}

void
CQFileMgrToolbar::
setIconSize(const QSize &s)
{
  iconSize_ = s;

  updateIconSize();
}

void
CQFileMgrToolbar::
updateIconSize()
{
  auto s = iconSize();

  for (auto *button : buttons_)
    button->setIconSize(s);
}

void
CQFileMgrToolbar::
showIcon(IconType type, bool show)
{
  if (type == ADD_PLACE_BUTTON)
    addButton_->setVisible(show);
}

void
CQFileMgrToolbar::
updateState()
{
  imageButton_ ->setChecked(mgr_->getShowImages());
  hiddenButton_->setChecked(mgr_->getShowHidden());
}

QSize
CQFileMgrToolbar::
sizeHint() const
{
  int is = iconSize().height();

  return QSize(100, is + 4);
}
