#ifndef CQZOOM_CURSOR_H
#define CQZOOM_CURSOR_H

// A Zoom Cursor is a small overlay displaying the original
// unzoomed image when displaying a zoomed image

#include <CImageLib.h>
#include <CAutoPtr.h>

class CQZoomCursor {
 public:
  enum Shape {
    SQUARE,
    CIRCLE
  };

 private:
  CImagePtr image_;     // original unzoomed image
  CImagePtr zimage_;    // image for overlay
  CISize2D  size_;      // size of the display area
  Shape     shape_;     // shape of the display area
  CIPoint2D pos_;       // last displayed position of the area
  bool      pos_valid_; // last position valid state
  double    factor_;    // zoom factor of the current zoomed display

  CAutoPtr<int> cursor_map_x_;
  CAutoPtr<int> cursor_map_y_;
  bool          cursor_map_valid_;

 public:
  CQZoomCursor(CImagePtr image);

  void setImage(CImagePtr image);
  void setSize(const CISize2D &size);
  void setFactor(double factor);

  CImagePtr getImage(CIPoint2D &pos);

 private:
  void updateImage(const CIPoint2D &pos);

  void setCursorMap();

  double remapPos(double pos);
};

#endif
