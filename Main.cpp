
#include <QtGui>
#include <QtDeclarative>
#include <QGLWidget>

#include "Player.h"
#include "Media.h"
#include "YouTubeSearch.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    qmlRegisterType<Player>("OmniMedia", 1, 0, "Player");

    QDeclarativeView view;
    YouTubeSearch *youtube = new YouTubeSearch(&view);
    QDeclarativeContext *context = view.rootContext();
    context->setContextProperty("youtubeModel", youtube->model());

    QObject::connect(view.engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view.setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.setWindowTitle("OmniMedia");
    view.setSource(QUrl::fromLocalFile("Main.qml"));

    QObject *root = qobject_cast<QObject *>(view.rootObject());
    QObject *searchInput = root->findChild<QObject *>("searchInput");

    if (searchInput)
        QObject::connect(searchInput, SIGNAL(queryChanged(QString)), youtube, SLOT(search(QString)));

    view.show();
    return app.exec();
}
