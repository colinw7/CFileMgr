#ifndef CFILE_MGR_DETAILS_H
#define CFILE_MGR_DETAILS_H

class CFileMgr;
class CKeyEvent;
class CPixelRenderer;
class CFileMgrFile;

#include <CAlignType.h>
#include <CEvent.h>
#include <vector>

struct CFileMgrColumnData {
  std::string name;
  CHAlignType align { CHALIGN_TYPE_LEFT };

  CFileMgrColumnData(const std::string &name1, CHAlignType align1=CHALIGN_TYPE_LEFT) :
   name(name1), align(align1) {
  }
};

//---

class CFileMgrDetails {
 public:
  typedef std::vector<CFileMgrColumnData> ColumnList;

 public:
  CFileMgrDetails(CFileMgr *file_mgr);

 ~CFileMgrDetails();

  void setRenderer(CPixelRenderer *renderer);

  CPixelRenderer *getRenderer() const { return renderer_; }

  void updateFont();

  void redraw();
  void draw();

  void loadDir();

  void selectRowNotify(int row);
  void selectColumnNotify(int col);

  void activateRowNotify(int row);
  void activateColumnNotify(int col);

  void activate(CFileMgrFile *file);

  void setOffset(double) { }

  int getRowsHeight() { return 12; }

  void buttonPress(const CMouseEvent &) { }
  void buttonRelease(const CMouseEvent &) { }
  void buttonMotion(const CMouseEvent &) { }

  void keyPress  (const CKeyEvent &) { }
  void keyRelease(const CKeyEvent &) { }

  void pointerMotion(const CMouseEvent &) { }

  void setColumns(const ColumnList &names);

  void deleteAllRows();

  void addRow(CFileMgrFile *file);

  void getScrollLimits(double *start, double *end);
  void setScrollLimits(double start, double end);

  void updateScrollLimits();

 private:
  CFileMgrFile *rowToFile(int row);

 public:
  CFileMgr       *file_mgr_     { nullptr };
  CPixelRenderer *renderer_     { nullptr };
  double          offset_       { 0.0 };
  double          scroll_start_ { 0.0 };
  double          scroll_end_   { 0.0 };
  ColumnList      columns_;
};

#endif
