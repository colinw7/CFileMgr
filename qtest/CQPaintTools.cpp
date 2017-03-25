#include <CQPaintTools.h>
#include <CQPixmapCache.h>
#include <CQImageButton.h>
#include <CQWinWidget.h>
#include <QVBoxLayout>

#include <svg/region_svg.h>
#include <svg/pen_svg.h>
#include <svg/color_picker_svg.h>
#include <svg/fill_svg.h>
#include <svg/close_svg.h>

CQPaintToolsWindow::
CQPaintToolsWindow(QWidget *parent) :
 CQWinWidget(parent)
{
  setResizable(false);

  tools_ = new CQPaintTools;

  setChild(tools_);
}

CQPaintTools::
CQPaintTools(QWidget *parent) :
 QWidget(parent)
{
  setObjectName("paintTools");

  layout_ = new QVBoxLayout(this);
  layout_->setMargin(2); layout_->setSpacing(2);

  select_button_  = new CQImageButton(CQPixmapCacheInst->getIcon("REGION"));
  pen_button_     = new CQImageButton(CQPixmapCacheInst->getIcon("PEN"));
  dropper_button_ = new CQImageButton(CQPixmapCacheInst->getIcon("COLOR_PICKER"));
  fill_button_    = new CQImageButton(CQPixmapCacheInst->getIcon("FILL"));
  close_button_   = new CQImageButton(CQPixmapCacheInst->getIcon("CLOSE"));

  select_button_ ->setObjectName("select");
  pen_button_    ->setObjectName("pen");
  dropper_button_->setObjectName("dropper");
  fill_button_   ->setObjectName("fill");
  close_button_  ->setObjectName("close");

  connect(select_button_ , SIGNAL(pressed()), this, SIGNAL(select ()));
  connect(pen_button_    , SIGNAL(pressed()), this, SIGNAL(pen    ()));
  connect(dropper_button_, SIGNAL(pressed()), this, SIGNAL(dropper()));
  connect(fill_button_   , SIGNAL(pressed()), this, SIGNAL(fill   ()));
  connect(close_button_  , SIGNAL(pressed()), this, SIGNAL(close  ()));

  layout_->addWidget(select_button_ );
  layout_->addWidget(pen_button_    );
  layout_->addWidget(dropper_button_);
  layout_->addWidget(fill_button_   );
  layout_->addWidget(close_button_  );

  layout_->addStretch(1);
}

CQPaintTools::
~CQPaintTools()
{
}
