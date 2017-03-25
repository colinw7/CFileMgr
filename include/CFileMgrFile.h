#ifndef CFILE_MGR_FILE_H
#define CFILE_MGR_FILE_H

#include <CFile.h>
#include <CIBBox2D.h>
#include <CISize2D.h>
#include <CIPoint2D.h>
#include <CImagePtr.h>

class CFileMgr;
class CFileMgrDir;

class CFileMgrFile {
 private:
  CFileMgrDir *dir_;
  CFile        file_;
  CIBBox2D     bbox_;
  int          row_;
  CImagePtr    image_;
  CImagePtr    timage_;
  CISize2D     image_size_;
  bool         selected_;

 public:
  CFileMgrFile(CFileMgrDir *dir, const std::string &filename);
 ~CFileMgrFile();

  std::string getPathName() const;
  std::string getFileName() const;

  CFileType getFileType();

  void setBBox(const CIBBox2D &bbox);
  void setBBox(int x1, int y1, int x2, int y2);

  void setRow(int row);

  bool inside(const CIBBox2D &bbox) const;
  bool inside(const CIPoint2D &point) const;
  bool inside(int x, int y) const;

  int getRow() const { return row_; }

  CImagePtr getImage(const CISize2D &size);
  CImagePtr getImage(int width, int height);

  bool getSelected() const { return selected_; }
  void setSelected(bool flag);

  bool isDirectory();
  bool isParentDir();
  bool isHidden();
  bool isImage();

  std::string getPerm();
  std::string getSize();
  std::string getTime();
  std::string getType();

  std::string getValue(const std::string &name);
};

#endif
