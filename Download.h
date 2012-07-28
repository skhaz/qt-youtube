#ifndef _Download_h
#define _Download

#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>



class Download : public QObject
{
    public:
        explicit Download(QObject *parent = 0);

    private:
        Q_OBJECT

        QNetworkReply *reply;
        QFile *file;
};

#endif

