#include "mainwindow.h"
#include <core/application.h>


int main(int argc, char *argv[])
{
      Application application(argc, argv);
      Application::setApplicationDisplayName("DollyXs");
//      Application::setOrganizationName("DollyXs");

      MainWindow w;
      w.show();
      return application.exec();
}
