#include <QWidget>

#include <CQWinWidget.h>

class CQPaint;
class CQImageButton;
class QVBoxLayout;

class CQPaintTools : public QWidget {
  Q_OBJECT

 private:
  CQImageButton *select_button_;
  CQImageButton *pen_button_;
  CQImageButton *dropper_button_;
  CQImageButton *fill_button_;
  CQImageButton *close_button_;
  QVBoxLayout   *layout_;

 public:
  static CQWinWidget *createToolWindow(QWidget *parent);

  CQPaintTools(QWidget *parent = 0);
 ~CQPaintTools();

 signals:
  void select();
  void pen();
  void dropper();
  void fill();
  void close();
};

class CQPaintToolsWindow : public CQWinWidget {
  Q_OBJECT

 private:
  CQPaintTools *tools_;

 public:
  CQPaintToolsWindow(QWidget *parent = 0);

  CQPaintTools *getTools() const { return tools_; }
};
