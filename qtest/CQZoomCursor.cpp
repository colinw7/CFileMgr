#include <CQZoomCursor.h>

CQZoomCursor::
CQZoomCursor(CImagePtr image) :
 image_(image)
{
  setSize(CISize2D(48, 48));
}

void
CQZoomCursor::
setImage(CImagePtr image)
{
  image_ = image;

  pos_valid_ = false;
}

void
CQZoomCursor::
setSize(const CISize2D &size)
{
  if (size_ == size) return;

  size_ = size;

  CImageNoSrc src;

  zimage_ = CImageMgrInst->createImage(src);

  zimage_->setDataSize(size_.getWidth(), size_.getHeight());

  pos_valid_        = false;
  cursor_map_valid_ = false;
}

void
CQZoomCursor::
setFactor(double factor)
{
  factor_ = factor;

  pos_valid_ = false;
}

CImagePtr
CQZoomCursor::
getImage(CIPoint2D &pos)
{
  updateImage(pos);

  uint w = size_.getWidth ();
  uint h = size_.getHeight();

  pos = CIPoint2D(pos.x - w/2, pos.y - h/2);

  return zimage_;
}

void
CQZoomCursor::
updateImage(const CIPoint2D &pos)
{
  if (! cursor_map_valid_)
    setCursorMap();

  // depends on:
  //  size_, factor_, image_

  int w = size_.getWidth ();
  int h = size_.getHeight();

  bool use_box = false;

  CIBBox2D box(0, 0, w, h);

  if (pos_valid_) {
    int dx = pos.x - pos_.x;
    int dy = pos.y - pos_.y;

    if (dx < w && dy < h) {
      box.moveBy(CIVector2D(dx, dy));

      zimage_->scroll(-dx, -dy);

      //use_box = true;
    }
  }

  // get position in original image
  double ifactor = 1.0/factor_;

  int mx = int(ifactor*pos.x);
  int my = int(ifactor*pos.y);

  for (int i = 0, y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x, ++i) {
      if (use_box && box.inside(CIPoint2D(x, y)))
        continue;

      CRGBA rgba;

      if (cursor_map_x_[i] != -999 &&
          cursor_map_y_[i] != -999) {
        int x1 = mx + cursor_map_x_[i];
        int y1 = my + cursor_map_y_[i];

        // get pixel from orginal image and put in zoom image
        if (image_->validPixel(x1, y1)) {
          image_->getRGBAPixel(x1, y1, rgba);

          zimage_->setRGBAPixel(x, y, rgba);
        }
        else {
          rgba = CRGBA(0,0,0,0);

          zimage_->setRGBAPixel(x, y, rgba);
        }
      }
      else {
        rgba = CRGBA(0,0,0,0);

        zimage_->setRGBAPixel(x, y, rgba);
      }
    }
  }

  pos_       = pos;
  pos_valid_ = true;
}

void
CQZoomCursor::
setCursorMap()
{
  // dependent on:
  //   size_

  uint w = size_.getWidth ();
  uint h = size_.getHeight();

  cursor_map_x_.set(new int [w*h]);
  cursor_map_y_.set(new int [w*h]);

  double r = std::max(w, h)/2.0;

  double r2 = r*r;

  for (uint i = 0, y = 0; y < h; ++y) {
    double dy  = y - r;
    double dy2 = dy*dy;

    for (uint x = 0; x < w; ++x, ++i) {
      double dx  = x - r;
      double dx2 = dx*dx;

      double d = (dx2 + dy2)/r2;

      if (d <= 1) {
        cursor_map_x_[i] = int(dx > 0 ? remapPos(dx) : -remapPos(-dx));
        cursor_map_y_[i] = int(dy > 0 ? remapPos(dy) : -remapPos(-dy));
      }
      else {
        cursor_map_x_[i] = -999;
        cursor_map_y_[i] = -999;
      }
    }
  }

  cursor_map_valid_ = true;
}

double
CQZoomCursor::
remapPos(double pos)
{
  if (pos > 16)
    return 16 + sqrt(pos - 16);
  else
    return pos;
}
