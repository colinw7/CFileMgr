#include <QTreeView>

#include <string>

class CQFileModel;
class CQFileModelSort;
class CQFileItem;

class CQFileView : public QTreeView {
 public:
  CQFileView(QWidget *parent=0);

  void addSort();

  CQFileModel *model() { return model_; }

 private:
  CQFileModel     *model_;
  CQFileModelSort *sortModel_;
};
