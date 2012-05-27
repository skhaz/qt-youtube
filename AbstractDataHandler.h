#ifndef ABSTRACTDATAHANDLER_H
#define ABSTRACTDATAHANDLER_H

#include <QObject>
#include <QUrl>

class AbstractDataHandler : public QObject
{
    Q_OBJECT
public:
    explicit AbstractDataHandler(QObject *parent = 0);

    virtual void asyncStart(const QUrl& url) = 0;

signals:
    void completed(const QUrl& url);
};

#endif // ABSTRACTDATAHANDLER_H
