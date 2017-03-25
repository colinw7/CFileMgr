#ifndef CDIR_VIEW_H
#define CDIR_VIEW_H

#include <string>
#include <vector>
#include <sys/types.h>

class CDirView;
class CDirViewNotifier;

class CDirViewFactory {
 public:
  CDirViewFactory() { }

  virtual ~CDirViewFactory() { }

  virtual CDirView *createView(const std::string &dirname) = 0;
};

#define CDirViewInst CDirView::getInstance()

class CDirView {
 public:
  class File {
   public:
    File(const std::string &filename, bool selected=false) :
     filename_(filename), selected_(selected) {
    }

    virtual ~File() { }

    const std::string &getName() const { return filename_; }

    bool getSelected() const { return selected_; }

   private:
    friend class CDirView;

    void setSelected(bool selected=true) { selected_ = selected; }

   protected:
    std::string filename_;
    bool        selected_;
  };

  struct Place {
    std::string name;
    std::string location;

    Place(const std::string &name1="", const std::string &location1="") :
     name(name1), location(location1) {
    }

    bool operator==(const Place &place) {
      return (name == place.name && location == place.location);
    }
  };

  typedef std::vector<File *> FileList;

 public:
  static void setFactory(CDirViewFactory *factory);

  static CDirView *getInstance();

  virtual ~CDirView();

  virtual bool setDirName(const std::string &dirname);

  const std::string &getDirName() const { return dirname_; }

  bool getShowHidden() const { return show_hidden_; }

  void setShowHidden(bool show_hidden);

  uint getNumFiles() const { return files_.size(); }

  const FileList &getFiles() const { return files_; }

  const File &getFile(uint i) const { return *(files_[i]); }

  File *getFile(const std::string &filename);

  void selectAll(bool selected=false);

  void selectFile(const std::string &filename, bool selected=false);

  void activateFile(const std::string &filename);

  void undoDir();
  void redoDir();

  bool canUndoDir();
  bool canRedoDir();

  virtual void selectionChanged();

  virtual void fileActivated(const std::string &filename);

  void pollChanged();

  virtual void contentsChanged();

  void getPlaces(std::vector<Place> &places);

 protected:
  virtual bool setDirName1(const std::string &dirname);

  virtual bool loadFiles();

  virtual File *createFile(const std::string &filename);

 protected:
  CDirView(const std::string &dirname);

 protected:
  typedef std::vector<std::string> DirStack;

  static CDirViewFactory *factory_;
  static CDirView        *view_;

  std::string       dirname_;
  FileList          files_;
  bool              show_hidden_;
  CDirViewNotifier *notifier_;
  DirStack          undo_stack_;
  DirStack          redo_stack_;
};

#endif
