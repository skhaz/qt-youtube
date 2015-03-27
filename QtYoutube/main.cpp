#include "MainWindow.h"
#include <QApplication>
#include "YouTubeSearch.h"
#include "YouTubeDataHandler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
