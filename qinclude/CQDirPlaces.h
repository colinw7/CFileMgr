#ifndef CQDIR_PLACES_H
#define CQDIR_PLACES_H

#include <CDirView.h>

#include <QListWidget>

#include <list>
#include <map>
#include <string>

class CQDir;
class CQFloatEdit;
class QListWidgetItem;

class CQDirPlaces : public QListWidget {
  Q_OBJECT

 public:
  CQDirPlaces(QWidget *parent=NULL);
 ~CQDirPlaces();

  void init();

  void addPlace(const std::string &name, const std::string &dirName);

 private:
  void update();

  void mouseDoubleClickEvent(QMouseEvent *event) override;

  void contextMenuEvent(QContextMenuEvent *event) override;

 private slots:
  void placeSlot(QListWidgetItem *);

  void itemEditFinished(const QString &text);

  void menuDeleteSlot();

 private:
  using PlaceList = std::list<CDirView::Place>;
  using PlaceMap  = std::map<std::string,std::string>;

  PlaceList        places_;
  PlaceMap         place_map_;
  CQFloatEdit     *edit_ { nullptr };
  QListWidgetItem *edit_item_ { nullptr };
  QListWidgetItem *menu_item_ { nullptr };
};

#endif
