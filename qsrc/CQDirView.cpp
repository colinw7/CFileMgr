#include <CQDirView.h>
#include <QTimer>

void
CQDirViewFactory::
init()
{
  CDirView::setFactory(new CQDirViewFactory);
}

CDirView *
CQDirViewFactory::
createView(const std::string &dirname)
{
  return new CQDirView(dirname);
}

//-------

CQDirView::
CQDirView(const std::string &dirname) :
 CDirView(dirname)
{
  emit dirChangedSignal(QString(dirname.c_str()));

  QTimer *timer = new QTimer;

  connect(timer, SIGNAL(timeout()), this, SLOT(pollChangedSlot()));

  timer->start(500);
}

bool
CQDirView::
setDirName1(const std::string &dirname)
{
  if (! CDirView::setDirName1(dirname))
    return false;

  emit dirChangedSignal(QString(dirname.c_str()));

  return true;
}

void
CQDirView::
setShowImages(bool b)
{
  if (b != showImages_) {
    showImages_ = b;

    emit showImagesChangedSignal(showImages_);
  }
}

void
CQDirView::
selectionChanged()
{
  emit selectionChangedSignal();
}

void
CQDirView::
fileActivated(const std::string &filename)
{
  emit fileActivatedSignal(QString(filename.c_str()));
}

void
CQDirView::
pollChangedSlot()
{
  pollChanged();
}

void
CQDirView::
contentsChanged()
{
  CDirView::contentsChanged();

  emit dirChangedSignal(QString(getDirName().c_str()));
}
