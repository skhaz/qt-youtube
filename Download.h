#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>



class Download : public QObject
{
    Q_OBJECT
public:
    explicit Download(QObject *parent = 0);

private:
    QNetworkReply *reply;
    QFile *file;
};

#endif // DOWNLOAD_H
