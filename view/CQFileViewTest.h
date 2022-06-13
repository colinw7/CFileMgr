#include <QDialog>
#include <CDupFile/CDupFile.h>

class CQFileView;
class CQFileItem;

class CQFileViewTest : public QDialog {
  Q_OBJECT

 public:
  enum CheckType {
    CHECK_DUPLICATES = (1<<0),
    CHECK_BAD_NAME   = (1<<1)
  };

 public:
  CQFileViewTest(const std::string &dirName=".");

  void setCheckTypes(uint checkTypes) { checkTypes_ = checkTypes; }

  void init();

  void addFile(CQFileItem *file);

  QSize sizeHint() const override;

 private:
  typedef std::vector<std::string> StringArray;

  std::string  dirName_;
  CQFileView  *view_;
  uint         checkTypes_;
  CQFileView  *infoView_;
  CDupFile     dupFile_;
  StringArray  badFiles_;
};
