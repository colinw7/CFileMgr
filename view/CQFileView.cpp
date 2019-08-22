#include <CQFileView.h>
#include <CQFileModel.h>

CQFileView::
CQFileView(QWidget *parent) :
 QTreeView(parent)
{
  setSelectionMode(QAbstractItemView::ExtendedSelection);

  model_ = new CQFileModel;

  setModel(model_);
}

void
CQFileView::
addSort()
{
  sortModel_ = new CQFileModelSort;

  sortModel_->setSourceModel(model_);

  setModel(sortModel_);

  setSortingEnabled(true);
}
