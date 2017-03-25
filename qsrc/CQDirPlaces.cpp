#include <CQDirPlaces.h>
#include <CQDirView.h>
#include <CQFloatEdit.h>
#include <CConfig.h>
#include <CDir.h>
#include <CStrUtil.h>

#include <QMouseEvent>
#include <QMenu>

CQDirPlaces::
CQDirPlaces(QWidget *parent) :
 QListWidget(parent), edit_(0)
{
  setObjectName("dirPlaces");

  connect(this, SIGNAL(itemClicked(QListWidgetItem *)),
          this, SLOT(placeSlot(QListWidgetItem *)));

  setContextMenuPolicy(Qt::DefaultContextMenu);

  init();
}

CQDirPlaces::
~CQDirPlaces()
{
}

void
CQDirPlaces::
init()
{
  std::vector<CDirView::Place> places;

  CQDirViewInst->getPlaces(places);

  clear();

  uint num_places = places.size();

  for (uint i = 0; i < num_places; ++i) {
    const CDirView::Place &place = places[i];

    addItem(place.name.c_str());

    place_map_[place.name] = place.location;

    places_.push_back(place);
  }
}

void
CQDirPlaces::
update()
{
  CConfig config("places");

  PlaceList::const_iterator p1, p2;

  std::string place_str;

  for (p1 = places_.begin(), p2 = places_.end(); p1 != p2; ++p1) {
    const CDirView::Place &place = *p1;

    if (! place_str.empty())
      place_str += ",";

    place_str += place.name + "@" + place.location;
  }

  config.setValue("places", place_str);
}

void
CQDirPlaces::
addPlace(const std::string &name, const std::string &dirname)
{
  if (place_map_.find(name) != place_map_.end())
    return;

  addItem(name.c_str());

  place_map_[name] = dirname;

  CDirView::Place place;

  place.name     = name;
  place.location = dirname;

  places_.push_back(place);

  update();
}

void
CQDirPlaces::
placeSlot(QListWidgetItem *item)
{
  QString text = item->text();

  std::string dirname = place_map_[text.toStdString()];

  CDirViewInst->setDirName(dirname);

  if (edit_ && text != edit_->text())
    edit_->hide();
}

void
CQDirPlaces::
mouseDoubleClickEvent(QMouseEvent *event)
{
  if (edit_ == 0) {
    edit_ = new CQFloatEdit(this);

    connect(edit_, SIGNAL(valueChanged(const QString &)),
            this, SLOT(itemEditFinished(const QString &)));
  }

  const QPoint &pos = event->pos();

  edit_item_ = itemAt(pos);

  if (! edit_item_) return;

  QRect rect = visualItemRect(edit_item_);

  rect.adjust(-2, -2, 2, 2);

  edit_->display(rect, edit_item_->text());
}

void
CQDirPlaces::
itemEditFinished(const QString &text)
{
  if (! edit_item_) return;

  std::string old_name = edit_item_->text().toStdString();
  std::string dirname  = place_map_[old_name];
  std::string new_name = text.toStdString();

  if (new_name.empty()) return;

  if (place_map_.find(new_name) != place_map_.end())
    return;

  CDirView::Place old_place(old_name, dirname);
  CDirView::Place new_place(new_name, dirname);

  places_   .remove(old_place);
  place_map_.erase (old_name);

  edit_item_->setText(new_name.c_str());

  place_map_[new_name] = dirname;

  places_.push_back(new_place);

  edit_item_ = 0;

  update();
}

void
CQDirPlaces::
contextMenuEvent(QContextMenuEvent *event)
{
  const QPoint &pos = event->pos();

  menu_item_ = itemAt(pos);

  if (! menu_item_) return;

  QMenu *menu = new QMenu;

  QAction *delAction = new QAction("Delete", this);

  connect(delAction, SIGNAL(triggered()),
          this, SLOT(menuDeleteSlot()));

  menu->addAction(delAction);

  menu->popup(event->globalPos());
}

void
CQDirPlaces::
menuDeleteSlot()
{
  if (! menu_item_) return;

  std::string name     = menu_item_->text().toStdString();
  std::string dirname  = place_map_[name];

  std::string place = name + "@" + dirname;

  places_   .remove(place);
  place_map_.erase (name);

  delete menu_item_;

  menu_item_ = 0;

  update();
}
