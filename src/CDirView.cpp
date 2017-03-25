#include <CDirView.h>
#include <CDirFiles.h>
#include <CDir.h>
#include <CFuncs.h>
#include <CConfig.h>
#include <CStrUtil.h>
#include <CFileNotify.h>

#include <iostream>
#include <cassert>

CDirViewFactory *CDirView::factory_ = 0;
CDirView        *CDirView::view_    = 0;

class CDirViewNotifier : public CFileNotifier {
 public:
  CDirViewNotifier(CDirView *dirview, const std::string &dirname) :
   CFileNotifier(dirname.c_str()), dirview_(dirview), debug_(false) {
  }

  void exec(uint flags) {
    if (debug_)
      std::cerr << getFilename() << ": " << CFileNotifyMgr::flagsToString(flags) << std::endl;

    dirview_->contentsChanged();
  }

 private:
  CDirView *dirview_;
  bool      debug_;
};

void
CDirView::
setFactory(CDirViewFactory *factory)
{
  assert(view_ == 0);

  factory_ = factory;
}

CDirView *
CDirView::
getInstance()
{
  if (! view_) {
    if (factory_)
      view_ = factory_->createView(".");
    else
      view_ = new CDirView(".");
  }

  return view_;
}

CDirView::
CDirView(const std::string &dirname) :
 dirname_(""), show_hidden_(false), notifier_(0)
{
  setDirName1(dirname);
}

CDirView::
~CDirView()
{
  for (auto &f : files_)
    delete f;
}

bool
CDirView::
setDirName(const std::string &dirname)
{
  std::string dirname1 = dirname_;

  bool rc = setDirName1(dirname);

  if (rc) {
    redo_stack_.clear();
    undo_stack_.push_back(dirname1);
  }

  return rc;
}

bool
CDirView::
setDirName1(const std::string &dirname)
{
  if (! CDir::enter(dirname))
    return false;

  dirname_ = CDir::getCurrent();

  CDir::leave();

  if (! loadFiles())
    return false;

  if (notifier_) {
    CFileNotifyMgrInst->removeNotifier(notifier_);

    delete notifier_;
  }

  notifier_ = new CDirViewNotifier(this, dirname_);

  CFileNotifyMgrInst->addNotifier(notifier_);

  return true;
}

bool
CDirView::
loadFiles()
{
  if (! CDir::enter(dirname_))
    return false;

  CDirFiles dir;

  CDirFiles::iterator p1, p2;

  files_.clear();

  for (p1 = dir.begin(), p2 = dir.end(); p1 != p2; ++p1) {
    const std::string &filename = (*p1)->getName();

    if (! show_hidden_ && filename[0] == '.') continue;

    File *file = createFile(filename);

    files_.push_back(file);
  }

  CDir::leave();

  return true;
}

void
CDirView::
undoDir()
{
  if (! canUndoDir()) return;

  std::string dirname = undo_stack_.back();

  undo_stack_.pop_back();
  redo_stack_.push_back(dirname_);

  setDirName1(dirname);
}

void
CDirView::
redoDir()
{
  if (! canRedoDir()) return;

  std::string dirname = redo_stack_.back();

  redo_stack_.pop_back();
  undo_stack_.push_back(dirname_);

  setDirName1(dirname);
}

bool
CDirView::
canUndoDir()
{
  return ! undo_stack_.empty();
}

bool
CDirView::
canRedoDir()
{
  return ! redo_stack_.empty();
}
void
CDirView::
setShowHidden(bool show_hidden)
{
  if (show_hidden != show_hidden_) {
    show_hidden_ = show_hidden;

    setDirName(dirname_);
  }
}

CDirView::File *
CDirView::
getFile(const std::string &filename)
{
  FileList::const_iterator p1, p2;

  for (p1 = files_.begin(), p2 = files_.end(); p1 != p2; ++p1) {
    File *file = *p1;

    if (file->getName() == filename)
      return file;
  }

  return 0;
}

void
CDirView::
selectAll(bool selected)
{
  FileList::const_iterator p1, p2;

  for (p1 = files_.begin(), p2 = files_.end(); p1 != p2; ++p1) {
    File *file = *p1;

    file->setSelected(selected);
  }

  selectionChanged();
}

void
CDirView::
selectFile(const std::string &filename, bool selected)
{
  File *file = getFile(filename);

  if (file) {
    file->setSelected(selected);

    selectionChanged();
  }
}

void
CDirView::
activateFile(const std::string &filename)
{
  File *file = getFile(filename);

  if (file) {
    std::string path = dirname_ + "/" + filename;

    if (CFile::isDirectory(path))
      setDirName(filename);
    else
      activateFile(filename);
  }
}

CDirView::File *
CDirView::
createFile(const std::string &filename)
{
  return new File(filename, false);
}

void
CDirView::
selectionChanged()
{
}

void
CDirView::
fileActivated(const std::string &filename)
{
  std::cerr << "Activate: " << filename;
}

void
CDirView::
pollChanged()
{
  CFileNotifyMgrInst->update(100);
}

void
CDirView::
contentsChanged()
{
  loadFiles();
}

void
CDirView::
getPlaces(std::vector<Place> &places)
{
  places.clear();

  CConfig config("places");

  std::string place_list;

  config.getValue("places", place_list);

  std::vector<std::string> placeStrs;

  CStrUtil::addFields(place_list, placeStrs, ",");

  if (! placeStrs.empty()) {
    uint numPlaceStrs = placeStrs.size();

    for (uint i = 0; i < numPlaceStrs; ++i) {
      const std::string &placeStr = placeStrs[i];

      std::vector<std::string> fields;

      fields.clear();

      CStrUtil::addFields(placeStr, fields, "@");

      if (fields.size() != 2) continue;

      places.push_back(Place(fields[0], fields[1]));
    }
  }

  if (places.empty())
    places.push_back(Place("Home", CDir::getHome()));
}
