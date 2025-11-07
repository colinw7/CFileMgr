#ifndef CIMAGE_THUMBNAIL_H
#define CIMAGE_THUMBNAIL_H

#include <CImagePtr.h>
#include <string>
#include <sys/types.h>

#define CThumbnailMgrInst CThumbnailMgr::getInstance()

class CThumbnailMgr {
 public:
  static CThumbnailMgr *getInstance();

  CImagePtr getImage(const std::string &filename, uint width, uint height);

 private:
  CThumbnailMgr();
 ~CThumbnailMgr();

 private:
  std::string image_dir_;
  bool        valid_;
};

#endif
