#ifndef _Download_h
#define _Download_h

#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>



class Download : public QObject
{
    public:
        explicit Download(QObject *parent = 0);

    public slots:

    private:
        Q_OBJECT

        QNetworkReply *reply;

        QFile *file;
};

// Q_DECLARE_METATYPE(Download)

#endif
