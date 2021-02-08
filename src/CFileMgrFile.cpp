#include <CFileMgrFile.h>
#include <CFileMgr.h>

#include <CStrUtil.h>
#include <CDir.h>
#include <CFileUtil.h>
#include <CTime.h>
#include <CFuncs.h>
#include <CImageMgr.h>
#include <CImage.h>

CFileMgrFile::
CFileMgrFile(CFileMgrDir *dir, const std::string &filename) :
 dir_(dir), file_(filename)
{
}

CFileMgrFile::
~CFileMgrFile()
{
}

std::string
CFileMgrFile::
getPathName() const
{
  return file_.getPath();
}

std::string
CFileMgrFile::
getFileName() const
{
  return file_.getName();
}

CFileType
CFileMgrFile::
getFileType()
{
  return CFileUtil::getType(&file_);
}

CImagePtr
CFileMgrFile::
getImage(const CISize2D &size)
{
  return getImage(size.width, size.height);
}

CImagePtr
CFileMgrFile::
getImage(int width, int height)
{
  if (! file_.isRegular())
    return CImagePtr();

  if (! image_.isValid()) {
    CImageFileSrc src(file_);

    image_ = CImageMgrInst->createImage(src);

    if (dir_->getFileMgr()->getForceRGB()) {
      if (image_.isValid() && image_->hasColormap())
        image_->convertToRGB();
    }
  }

  if (! image_.isValid())
    return CImagePtr();

  if (width  != image_size_.width || height != image_size_.height) {
    image_size_.set(width, height);

    timage_ = image_->resizeKeepAspect(image_size_);
  }

  return timage_;
}

void
CFileMgrFile::
setBBox(const CIBBox2D &bbox)
{
  bbox_ = bbox;
}

void
CFileMgrFile::
setBBox(int x1, int y1, int x2, int y2)
{
  bbox_.set(x1, y1, x2, y2);
}

void
CFileMgrFile::
setRow(int row)
{
  row_ = row;
}

bool
CFileMgrFile::
inside(int x, int y) const
{
  CIPoint2D point(x, y);

  return inside(point);
}

bool
CFileMgrFile::
inside(const CIPoint2D &point) const
{
  return bbox_.inside(point);
}

bool
CFileMgrFile::
inside(const CIBBox2D &bbox) const
{
  return bbox.overlaps(bbox_);
}

void
CFileMgrFile::
setSelected(bool selected)
{
  selected_ = selected;
}

bool
CFileMgrFile::
isDirectory()
{
  return file_.isDirectory();
}

bool
CFileMgrFile::
isParentDir()
{
  return (file_.getName() == "..");
}

bool
CFileMgrFile::
isHidden()
{
  return file_.isHidden();
}

bool
CFileMgrFile::
isImage()
{
  return (getFileType() & CFILE_TYPE_IMAGE);
}

std::string
CFileMgrFile::
getPerm()
{
  return file_.getModeString();
}

std::string
CFileMgrFile::
getSize()
{
  return CStrUtil::toString(file_.getSize());
}

std::string
CFileMgrFile::
getTime()
{
  CTime time(file_.getMTime());

  return time.getLsTime(false);
}

std::string
CFileMgrFile::
getType()
{
  return CFileUtil::getTypeStr(getFileType());
}

std::string
CFileMgrFile::
getValue(const std::string &name)
{
  if      (name == "Name")
    return getFileName();
  else if (name == "Perm")
    return getPerm();
  else if (name == "Size")
    return getSize();
  else if (name == "Date")
    return getTime();
  else if (name == "Type")
    return getType();
  else
    return "";
}
