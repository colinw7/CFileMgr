#include <CImageThumbnail.h>
#include <CImageLib.h>
#include <CEncode64.h>
#include <CFile.h>
#include <CDir.h>
#include <CEnv.h>

CThumbnailMgr *
CThumbnailMgr::
getInstance()
{
  static CThumbnailMgr *instance;

  if (! instance)
    instance = new CThumbnailMgr;

  return instance;
}

CThumbnailMgr::
CThumbnailMgr()
{
  if (CEnvInst.exists("THUMBNAIL_DIR"))
    image_dir_ = CEnvInst.get("THUMBNAIL_DIR");
  else
    image_dir_ = "";

  if (image_dir_ == "")
    image_dir_ = CDir::getHome() + "/.thumbnails";

  valid_ = (CFile::exists(image_dir_) && CFile::isDirectory(image_dir_));

  if (! valid_)
    std::cerr << image_dir_ << " is invalid directory\n";
}

CThumbnailMgr::
~CThumbnailMgr()
{
}

CImagePtr
CThumbnailMgr::
getImage(const std::string &filename, uint width, uint height)
{
  if (! valid_)
    return CImagePtr();

  if (width < 1 || height < 1) {
    std::cerr << "Invalid size " << width << "x" << height << "\n";
    return CImagePtr();
  }

  CFile imgfile(filename);

  if (! imgfile.exists() && ! imgfile.isRegular()) {
    std::cerr << "Invalid image file " << filename << "\n";
    return CImagePtr();
  }

  int imgTime = imgfile.getMTime();

  char sizeDir[32];

  sprintf(sizeDir, "%ux%u", width, height);

  std::string dirname = image_dir_ + "/" + sizeDir;

  if (! CFile::isDirectory(dirname))
    CDir::makeDir(dirname);

  std::string path = imgfile.getPath();

  std::string home = CDir::getHome() + "/";

  if (path.find(home) == 0)
    path = path.substr(home.size());

  std::string epath = CEncode64Inst->encode(path);

  std::string thumbname = dirname + "/" + epath + ".png";

  CFile thumbfile(thumbname);

  if (thumbfile.exists()) {
    int thumbTime = thumbfile.getMTime();

    if (imgTime < thumbTime) {
      CImageFileSrc src(thumbfile);

      return CImageMgrInst->createImage(src);
    }
  }

  CImageFileSrc src(imgfile);

  CImagePtr image = CImageMgrInst->createImage(src);

  if (! image) {
    std::cerr << "Invalid image file " << filename << "\n";
    return CImagePtr();
  }

  if (image->getWidth() > width || image->getHeight() > height) {
    image->convertToRGB();

    image = image->resizeKeepAspect(int(width), int(height));
  }

  image->writePNG(&thumbfile);

  return image;
}
