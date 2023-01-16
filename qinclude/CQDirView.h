#ifndef CQDIR_VIEW_H
#define CQDIR_VIEW_H

#include <QObject>

#include <CDirView.h>

#define CQDirViewInst dynamic_cast<CQDirView *>(CDirViewInst)

class CQDirViewFactory : public CDirViewFactory {
 public:
  static void init();

  CDirView *createView(const std::string &dirname) override;
};

class CQDirView : public QObject, public CDirView {
  Q_OBJECT

 public:
 ~CQDirView() { }

  bool getShowImages() const { return showImages_; }
  void setShowImages(bool b);

 protected:
  friend class CQDirViewFactory;

  CQDirView(const std::string &dirname);

  bool setDirName1(const std::string &dirname) override;

  void selectionChanged() override;

  void fileActivated(const std::string &filename) override;

  void contentsChanged() override;

 signals:
  void dirChangedSignal(const QString &dirname);

  void fileActivatedSignal(const QString &filename);

  void selectionChangedSignal();

  void showImagesChangedSignal(bool);

 private slots:
  void pollChangedSlot();

 private:
  bool showImages_ { false };
};

#endif
