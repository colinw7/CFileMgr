#include <CQFileMgrTest.h>
#include <CQApp.h>
#include <CQDirView.h>

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  CQDirViewFactory::init();

  CQFileMgrTest *shell = new CQFileMgrTest();

  shell->init();

  shell->show();

  return app.exec();
}
