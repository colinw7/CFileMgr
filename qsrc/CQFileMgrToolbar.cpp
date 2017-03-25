#include <CQFileMgrToolbar.h>

#include <CConfig.h>
#include <CDir.h>
#include <CStrUtil.h>
#include <CQFileMgr.h>
#include <CQImageButton.h>

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
 QWidget(0), mgr_(mgr)
{
  setObjectName("fileMgrToolbar");

  init();

  connect(mgr_, SIGNAL(stateChanged()), this, SLOT(updateState()));
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

  backButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("BACK"));
  backButton_->setToolTip("Previous Dir");
  connect(backButton_, SIGNAL(clicked()), mgr_, SLOT(undoDir()));

  forwardButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("FORWARD"));
  forwardButton_->setToolTip("Next Dir");
  connect(forwardButton_, SIGNAL(clicked()), mgr_, SLOT(redoDir()));

  //CQImageButton *up_button = new CQImageButton(QPixmap((const char **) up_data));
  //connect(up_button, SIGNAL(clicked()), mgr_, SLOT(parentDir()));

  iconsButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("ICONS"));
  iconsButton_->setToolTip("Icons View");
  connect(iconsButton_, SIGNAL(clicked()), mgr_, SLOT(iconsView()));

  detailsButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("DETAILS"));
  detailsButton_->setToolTip("Details View");
  connect(detailsButton_, SIGNAL(clicked()), mgr_, SLOT(detailsView()));

  stripButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("STRIP"));
  stripButton_->setToolTip("Strip View");
  connect(stripButton_, SIGNAL(clicked()), mgr_, SLOT(filmstripView()));

  imageButton_  = new CQImageButton(CQPixmapCacheInst->getIcon("IMAGE"));
  imageButton_->setToolTip("Show Images");
  imageButton_->setCheckable(true);
  connect(imageButton_, SIGNAL(clicked(bool)), mgr_, SLOT(setShowImages(bool)));

  hiddenButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("HIDDEN"));
  hiddenButton_->setToolTip("Show Hidden");
  hiddenButton_->setCheckable(true);
  connect(hiddenButton_, SIGNAL(clicked(bool)), mgr_, SLOT(setShowHidden(bool)));

  refreshButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("REFRESH"));
  refreshButton_->setToolTip("Refresh View");
  connect(refreshButton_, SIGNAL(clicked()), mgr_, SLOT(refresh()));

  addButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("FAVORITES"));
  addButton_->setToolTip("Add to Favorties");
  connect(addButton_, SIGNAL(clicked()), mgr_, SLOT(addToPlaces()));

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

  setFixedHeight(34);
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
