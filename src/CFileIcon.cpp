#include <CFileIcon.h>
#include <CFileUtil.h>
#include <CImageThumbnail.h>

CFileIconMgr *
CFileIconMgr::
getInstance()
{
  static CFileIconMgr *instance;

  if (! instance)
    instance = new CFileIconMgr;

  return instance;
}

CFileIconMgr::
CFileIconMgr() :
 config_("CFileIcon"), icon_type_(SMALL_ICONS), icon_size_(1,1), show_image_(false)
{
  icon_type_ = SMALL_ICONS;

  setIconType(LARGE_ICONS);
}

void
CFileIconMgr::
setIconType(IconType icon_type)
{
  if (icon_type_ == icon_type)
    return;

  icon_type_ = icon_type;

  //------

  updateIconSize();
}

int
CFileIconMgr::
getSmallIconSize() const
{
  int icon_size;

  if (! config_.getValue("smallIconSize", &icon_size))
    icon_size = 32;

  return icon_size;
}

void
CFileIconMgr::
setSmallIconSize(int s)
{
  config_.setValue("smallIconSize", s);

  updateIconSize();
}

int
CFileIconMgr::
getLargeIconSize() const
{
  int icon_size;

  if (! config_.getValue("largeIconSize", &icon_size))
    icon_size = 64;

  return icon_size;
}

void
CFileIconMgr::
setLargeIconSize(int s)
{
  config_.setValue("largeIconSize", s);

  updateIconSize();
}

void
CFileIconMgr::
updateIconSize()
{
  int icon_size;

  if (icon_type_ == LARGE_ICONS)
    icon_size = getLargeIconSize();
  else
    icon_size = getSmallIconSize();

  setIconSize(CISize2D(icon_size, icon_size));
}

void
CFileIconMgr::
setIconSize(const CISize2D &icon_size)
{
  if (icon_size == icon_size_)
    return;

  icon_size_ = icon_size;

  clearIcons();
}

void
CFileIconMgr::
setShowImage(bool show_image)
{
  if (show_image == show_image_)
    return;

  show_image_ = show_image;
}

CImagePtr
CFileIconMgr::
getImage(const std::string &fileName)
{
  CFileType type = CFileUtil::getType(fileName);

  if (getShowImage() && CFileUtil::isImageType(type))
    return getContents(fileName);
  else
    return getImage(type);
}

CImagePtr
CFileIconMgr::
getNormalImage(const std::string &fileName)
{
  CFileType type = CFileUtil::getType(fileName);

  if (getShowImage() && CFileUtil::isImageType(type))
    return getNormalContents(fileName);
  else
    return getImage(type);
}

CImagePtr
CFileIconMgr::
getImage(CFileType type)
{
  if (icon_type_ == LARGE_ICONS)
    return getLargeImage(type);
  else
    return getSmallImage(type);
}

CImagePtr
CFileIconMgr::
getContents(const std::string &fileName)
{
  return CThumbnailMgrInst->getImage(fileName, uint(getIconWidth()), uint(getIconHeight()));

  //CImageSizedFileSrc src(fileName, getIconWidth(), getIconHeight(), true);

  //return CImageMgrInst->lookupImage(src);
}

CImagePtr
CFileIconMgr::
getNormalContents(const std::string &fileName)
{
  CImageFileSrc src(fileName);

  return CImageMgrInst->lookupImage(src);
}

CImagePtr
CFileIconMgr::
getLargeImage(CFileType type)
{
  std::string type_str = CFileUtil::getTypeStr(type);

  ImageMap::iterator pimage1 = large_image_map_.find(type_str);

  if (pimage1 != large_image_map_.end())
    return (*pimage1).second;

  //------

  CImagePtr image;

  std::string filename;

  if (config_.getValue("large/" + type_str, filename)) {
    CImageFileSrc src(filename);

    image = CImageMgrInst->createImage(src);

    if (! image)
      std::cerr << "Failed to read " << filename << std::endl;
  }
  else
    std::cerr << "No file for large/" << type_str << std::endl;

  if (! image) {
    if (config_.getValue("large/default", filename)) {
      CImageFileSrc src(filename);

      image = CImageMgrInst->createImage(src);

      if (! image)
        std::cerr << "Failed to read " << filename << std::endl;
    }
    else
      std::cerr << "No file for large/default" << std::endl;
  }

  if (image) {
    if (getIconWidth () != int(image->getWidth ()) ||
        getIconHeight() != int(image->getHeight()))
      image = image->resizeKeepAspect(getIconSize());
  }

  large_image_map_[type_str] = image;

  return image;
}

CImagePtr
CFileIconMgr::
getSmallImage(CFileType type)
{
  std::string type_str = CFileUtil::getTypeStr(type);

  ImageMap::iterator pimage1 = small_image_map_.find(type_str);

  if (pimage1 != small_image_map_.end())
    return (*pimage1).second;

  //------

  CImagePtr image;

  std::string filename;

  if (config_.getValue("small/" + type_str, filename)) {
    CImageFileSrc src(filename);

    image = CImageMgrInst->createImage(src);

    if (! image)
      std::cerr << "Failed to read " << filename << std::endl;
  }
  else
    std::cerr << "No file for small/" << type_str << std::endl;

  if (! image) {
    if (config_.getValue("small/default", filename)) {
      CImageFileSrc src(filename);

      image = CImageMgrInst->createImage(src);

      if (! image)
        std::cerr << "Failed to read " << filename << std::endl;
    }
    else
      std::cerr << "No file for small/default" << std::endl;
  }

  if (image) {
    if (getIconWidth () != int(image->getWidth ()) ||
        getIconHeight() != int(image->getHeight()))
      image = image->resizeKeepAspect(getIconSize());
  }

  small_image_map_[type_str] = image;

  return image;
}

void
CFileIconMgr::
clearIcons()
{
  large_image_map_.clear();
  small_image_map_.clear();
}
