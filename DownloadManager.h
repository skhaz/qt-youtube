#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QDesktopServices>
#include <QQueue>
#include <QFileInfo>
#include <QStringList>
#include <QVariant>
#include <QPair>
#include <QUrl>

#define CACHE_SIZE 128 * 1024 * 1024
#define MAX_DOWNLOADS 16

class DownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = 0);

public slots:
    void append(const QStringList& urls);
    void append(const QUrl& url, const QString& filename);
    void remove(const QString& filename);

protected slots:
    void nextDownload();
    void readReady();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void error(QNetworkReply::NetworkError code);
    void finished();

private:
    Q_DISABLE_COPY(DownloadManager)
    QNetworkAccessManager manager;
    QNetworkDiskCache cache;
    QQueue<QPair<QUrl, QString> > queue;
    QHash<QString, QFile *> downloads;
};

#endif // DOWNLOADMANAGER_H
