#ifndef CQFILEMGR_H
#define CQFILEMGR_H

#include <QWidget>

#include <CIPoint2D.h>
#include <CIBBox2D.h>
#include <CFileMgrLib.h>

class CQFileMgrIcons;
class CQFileMgrFilmstripImage;
class CQFileMgrFilmstripIcons;
class CQFileMgrDetails;
class CQFileMgrToolbar;
class CQFileMgrInfo;
class CQDirBar;
class CQDirPlaces;
class QTreeWidget;

class CQFileMgr : public QWidget {
  Q_OBJECT

  Q_PROPERTY(int smallIconSize READ getSmallIconSize WRITE setSmallIconSize)
  Q_PROPERTY(int largeIconSize READ getLargeIconSize WRITE setLargeIconSize)
  Q_PROPERTY(int fontSizer     READ getFontSize      WRITE setFontSize)

 public:
  CQFileMgr(QWidget *parent=nullptr);

  virtual ~CQFileMgr();

  CFileMgr *getFileMgr() const { return filemgr_; }

  CQFileMgrDetails *getDetailsTable() const { return details_table_; }

  const std::string &getDirName() const;
  void setDirName(const std::string &dirName);

  //---

  int getSmallIconSize() const;
  void setSmallIconSize(int s);

  int getLargeIconSize() const;
  void setLargeIconSize(int s);

  int getFontSize() const;
  void setFontSize(int s);

  //---

  void showPlaces(bool show);

  void resize();
  void changed();
  void expose();

  void keyPress  (const CKeyEvent &event);
  void keyRelease(const CKeyEvent &event);

  void draw();

  void drawIcons();
  void drawFilmstripImage();
  void drawFilmstripIcons();
  void drawDetails();

  int getCanvasWidth() const;
  int getCanvasHeight() const;

  int getContentsWidth() const;
  int getContentsHeight() const;

  void setIconsView();
  void setFilmstripView();
  void setDetailsView();

  bool getSmallIcons();
  bool getShowImages();
  bool getShowHidden();

  void deselectAll();

  void selectInside(const CIBBox2D &bbox);

  void activate(const CIPoint2D &position);

  void activate(CFileMgrFile *file);

  virtual void activateFile(const std::string &filename);
  virtual void activateDir(const std::string &dirname);

  bool activateFileTypeFwd(const std::string &filename, CFileType type);

  virtual void activateFileType(const std::string &filename, CFileType type);

  virtual void changeDir(const std::string &dirName);

  void updateScrollbars();

  void scrolled();

  virtual void setTableColumns(const CFileMgrDetails::ColumnList &columns);
  virtual void deleteAllTableRows();
  virtual void addTableRow(CFileMgrFile *file, const std::vector<std::string> &values);

  void popupMenu(QWidget *widget, int x, int y);

 public Q_SLOTS:
  void updateSizes();

  void updateFilmstripSize();

  void undoDir();
  void redoDir();

  void parentDir();
  void setDir(const QString &dirName);

  void iconsView();
  void filmstripView();
  void detailsView();

  void setShowImages(bool show_images=true);
  void setSmallIcons(bool small_icons=true);
  void setShowHidden(bool show_hidden=true);

  void addToPlaces();

  void showInfo();

  void refresh();

 signals:
  void fileActivated(const QString &filename);
  void dirActivated(const QString &dirname);

  void dirChanged(const QString &dirName);

  void stateChanged();

 protected:
  CFileMgr                *filemgr_ { nullptr };
  CQFileMgrToolbar        *toolbar_ { nullptr };
  CQDirBar                *dirbar_ { nullptr };
  QWidget                 *places_frame_ { nullptr };
  CQDirPlaces             *places_ { nullptr };
  QWidget                 *icons_frame_ { nullptr };
  CQFileMgrIcons          *icons_canvas_ { nullptr };
  QWidget                 *filmstrip_frame_ { nullptr };
  CQFileMgrFilmstripImage *filmstrip_image_canvas_ { nullptr };
  CQFileMgrFilmstripIcons *filmstrip_icons_canvas_ { nullptr };
  QWidget                 *details_frame_ { nullptr };
  CQFileMgrDetails        *details_table_ { nullptr };
  CQFileMgrInfo           *info_ { nullptr };
  bool                     activated_ { false };
};

#endif
