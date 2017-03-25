#ifndef CFILE_ICON_H
#define CFILE_ICON_H

#include <CConfig.h>
#include <CISize2D.h>
#include <CImageLib.h>
#include <map>

#define CFileIconMgrInst CFileIconMgr::getInstance()

class CFileIconMgr {
 public:
  enum IconType {
    LARGE_ICONS,
    SMALL_ICONS
  };

 public:
  static CFileIconMgr *getInstance();

  void setIconType(IconType icon_type);

  void setIconSize(const CISize2D &icon_size);
  const CISize2D &getIconSize() const { return icon_size_; }

  int getSmallIconSize() const;
  void setSmallIconSize(int s);

  int getLargeIconSize() const;
  void setLargeIconSize(int s);

  void updateIconSize();

  int getIconWidth () const { return icon_size_.width ; }
  int getIconHeight() const { return icon_size_.height; }

  bool getShowImage() const { return show_image_; }

  void setShowImage(bool show_image);

  CImagePtr getImage(const std::string &fileName);
  CImagePtr getNormalImage(const std::string &fileName);
  CImagePtr getImage(CFileType type);

 private:
  CFileIconMgr();
 ~CFileIconMgr();

  // no implementation for singleton
  CFileIconMgr(const CFileIconMgr &mgr);
  CFileIconMgr &operator=(const CFileIconMgr &mgr);

  CImagePtr getContents(const std::string &fileName);
  CImagePtr getNormalContents(const std::string &fileName);

  CImagePtr getLargeImage(CFileType type);
  CImagePtr getSmallImage(CFileType type);

  void clearIcons();

 private:
  typedef std::map<std::string,CImagePtr> ImageMap;

  CConfig  config_;
  IconType icon_type_;
  CISize2D icon_size_;
  bool     show_image_;
  ImageMap large_image_map_;
  ImageMap small_image_map_;
};

#endif
