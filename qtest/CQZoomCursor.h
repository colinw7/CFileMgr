#ifndef CQZOOM_CURSOR_H
#define CQZOOM_CURSOR_H

// A Zoom Cursor is a small overlay displaying the original
// unzoomed image when displaying a zoomed image

#include <QObject>
#include <CImageLib.h>

class CQZoomCursor : QObject {
  Q_OBJECT

  Q_PROPERTY(int    width  READ getWidth  WRITE setWidth )
  Q_PROPERTY(double factor READ getFactor WRITE setFactor)

 public:
  enum class Shape {
    SQUARE,
    CIRCLE
  };

 public:
  CQZoomCursor(CImagePtr image);

  CImagePtr getImage(CIPoint2D &pos);
  void setImage(CImagePtr image);

  const CISize2D &getSize() const { return size_; }
  void setSize(const CISize2D &size);

  int getWidth() const { return size_.getWidth(); }
  void setWidth(int s) { setSize(CISize2D(s, s)); }

  double getFactor() const { return factor_; }
  void setFactor(double factor);

 private:
  void updateImage(const CIPoint2D &pos);

  void setCursorMap();

  double remapPos(double pos);

 private:
  CImagePtr        image_;                   // original unzoomed image
  CImagePtr        zimage_;                  // image for overlay
  CISize2D         size_ { 0, 0 };           // size of the display area
  Shape            shape_ { Shape::CIRCLE }; // shape of the display area
  CIPoint2D        pos_;                     // last displayed position of the area
  bool             pos_valid_ { false };     // last position valid state
  double           factor_ { 2.0 };          // zoom factor of the current zoomed display
  std::vector<int> cursor_map_x_;
  std::vector<int> cursor_map_y_;
  bool             cursor_map_valid_ { false };
};

#endif
