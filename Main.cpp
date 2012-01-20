
#include <QtGui>
#include <QtDeclarative>
#include <QGLWidget>

#include "Player.h"

#include "DownloadManager.h"

#include <QtDebug>
#include <QFile>
#include <QtXmlPatterns>
#include <QStringList>

#include "Media.h"
#include "MediaModel.h"

#include "YouTubeSearch.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    qmlRegisterType<Player>("OmniMedia", 1, 0, "Player");

    QDeclarativeView view;
    QObject::connect(view.engine(), SIGNAL(quit()), qApp, SLOT(quit()));

    YouTubeSearch *youtube = new YouTubeSearch(&view);
    youtube->search("starcraft");

    QDeclarativeContext *context = view.rootContext();
    context->setContextProperty("youtubeModel", youtube->model());

    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view.setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.setSource(QUrl::fromLocalFile("Main.qml"));
    view.setWindowTitle("OmniMedia");
    view.show();

    return app.exec();
}
