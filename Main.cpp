
#include <QtGui>
#include <QtDeclarative>
#include <QGLWidget>

#include "Player.h"
#include "Media.h"
#include "YouTubeSearch.h"



#include <iostream>
#include <fstream>
#include <iterator>
#include <iomanip>

#include <libtorrent/entry.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/lazy_entry.hpp>
#include <boost/filesystem/operations.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    qmlRegisterType<Player>("OmniMedia", 1, 0, "Player");
    qmlRegisterType<Media>("OmniMedia", 1, 0, "Media");

    using namespace libtorrent;
    session s;
    s.listen_on(std::make_pair(6881, 6889));
    add_torrent_params p;
    p.save_path = "./";
    p.ti = new torrent_info("/Users/Skhaz/Downloads/The.Big.Bang.Theory.S04E23.The.Engagement.Reaction.HDTV.XviD-FQM.[eztv].torrent");
    s.add_torrent(p);



    QDeclarativeView view;

    YouTubeSearch *youtube = new YouTubeSearch(&view);
    youtube->setContext(view.rootContext());

    QObject::connect(view.engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view.setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.setAttribute(Qt::WA_OpaquePaintEvent);
    view.setAttribute(Qt::WA_NoSystemBackground);
    view.viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    view.viewport()->setAttribute(Qt::WA_NoSystemBackground);
    view.setWindowTitle("OmniMedia");
    view.setSource(QUrl::fromLocalFile("./Main.qml"));

    QObject *root = qobject_cast<QObject *>(view.rootObject());
    QObject *searchInput = root->findChild<QObject *>("searchInput");

    if (searchInput)
        QObject::connect(searchInput, SIGNAL(queryChanged(QString)), youtube, SLOT(search(QString)));

    view.show();
    return app.exec();
}
