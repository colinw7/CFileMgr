#include <CQFileViewTest.h>
#include <CQFileView/CQFileView.h>
#include <CQFileView/CQFileModel.h>
#include <CQFileView/CQFileItem.h>
#include <CFileUtil/CFileUtil.h>

#include <sys/stat.h>

#include <iostream>

#include <QApplication>
#include <QVBoxLayout>
#include <QSplitter>
#include <QDateTime>

class CQDupFileItem : public CDupFile::File {
 public:
  CQDupFileItem(CQFileItem *item) :
   CDupFile::File(item->filename()), item_(item) {
  }

  CQFileItem *item() const { return item_; }

 private:
  CQFileItem *item_;
};

int
main(int argc, char **argv)
{
  QApplication app(argc, argv);

  auto f = app.font();
  f.setPointSizeF(20);
  app.setFont(f);

  uint checkTypes = 0;

  std::string dirName = ".";

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if      (strcmp(&argv[i][1], "duplicates") == 0)
        checkTypes |= CQFileViewTest::CHECK_DUPLICATES;
      else if (strcmp(&argv[i][1], "bad_names") == 0)
        checkTypes |= CQFileViewTest::CHECK_BAD_NAME;
      else
        std::cerr << "Bad arg '" << argv[i] << "'" << std::endl;
    }
    else
      dirName = argv[i];
  }

  auto *test = new CQFileViewTest(dirName);

  test->setCheckTypes(checkTypes);

  test->init();

  test->show();

  return app.exec();
}

class CQFileStatVisitor : public CQFileVisitor {
 public:
  CQFileStatVisitor(CQFileViewTest *test) :
   test_(test) {
  }

  void enterDir(CQFileItem *) {
    sizeStack_.push_back(size_);

    size_ = 0;
  }

  void leaveDir(CQFileItem *file) {
    file->setData("Size", QVariant(qulonglong(size_)));

    size_ = sizeStack_.back();

    sizeStack_.pop_back();
  }

  void visitFile(CQFileItem *file) {
    test_->addFile(file);

    struct stat fstat;

    lstat(file->filename().c_str(), &fstat);

    file->setData("Size", QVariant(qulonglong(fstat.st_size)));

    QDateTime timestamp;

    timestamp.setTime_t(uint(fstat.st_mtime));

    file->setData("Date", QVariant(timestamp));

    size_ += fstat.st_size;
  }

 private:
  CQFileViewTest     *test_;
  off_t               size_;
  std::vector<off_t>  sizeStack_;
};

CQFileViewTest::
CQFileViewTest(const std::string &dirName) :
 dirName_(dirName), checkTypes_(0)
{
}

void
CQFileViewTest::
init()
{
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(2); layout->setSpacing(2);

  view_ = new CQFileView;

  QSplitter *splitter = new QSplitter;

  splitter->setOrientation(Qt::Vertical);

  layout->addWidget(splitter);

  QStringList cols = QStringList() << "Name" << "Size" << "Date";

  if (checkTypes_ & CHECK_DUPLICATES)
    cols << "Duplicates";

  if (checkTypes_ & CHECK_BAD_NAME)
    cols << "Bad Name";

  CQFileColumns *columns = new CQFileColumns(cols);

  view_->model()->setColumns(columns);

  CQFileStatVisitor visitor(this);

  view_->model()->addDirFiles(dirName_, &visitor);

  splitter->addWidget(view_);

  view_->addSort();

  //----

  if (checkTypes_) {
    infoView_ = new CQFileView;

    CQFileColumns *infoColumns = new CQFileColumns(QStringList() << "Name" << "Type" << "Info");

    infoView_->model()->setColumns(infoColumns);

    if (checkTypes_ & CHECK_DUPLICATES) {
      dupFile_.checkFiles();

      const CDupFile::FilePairs &dupFiles = dupFile_.getDupFiles();

      for (CDupFile::FilePairs::const_iterator p = dupFiles.begin(); p != dupFiles.end(); ++p) {
        auto *file1 = static_cast<const CQDupFileItem *>((*p).first);
        auto *file2 = static_cast<const CQDupFileItem *>((*p).second);

        file1->item()->setData("Duplicates", file2->filename().c_str());
        file2->item()->setData("Duplicates", file1->filename().c_str());

        CQFileItem *item = infoView_->model()->addFile(file1->filename());

        item->setData("Type", QString("Dup"));
        item->setData("Info", QString(file2->filename().c_str()));
      }
    }

    if (checkTypes_ & CHECK_BAD_NAME) {
      for (StringArray::const_iterator p = badFiles_.begin(); p != badFiles_.end(); ++p) {
        const std::string &filename = *p;

        CQFileItem *item = infoView_->model()->addFile(filename);

        item->setData("Type", QString("Bad Name"));
        item->setData("Info", QString(""));
      }
    }

    splitter->addWidget(infoView_);
  }
}

void
CQFileViewTest::
addFile(CQFileItem *file)
{
  if (checkTypes_ & CHECK_DUPLICATES)
    dupFile_.addFile(new CQDupFileItem(file));

  if (checkTypes_ & CHECK_BAD_NAME) {
    bool isBadName = CFileUtil::isBadFilename(file->filename());

    file->setData("Bad Name", QVariant(isBadName));

    if (isBadName)
      badFiles_.push_back(file->filename());
  }
}

QSize
CQFileViewTest::
sizeHint() const
{
  return QSize(1024, 1024);
}
