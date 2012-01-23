#ifndef AbstractDataHandler_h
#define AbstractDataHandler_h

#include <QObject>
#include <QUrl>



class AbstractDataHandler : public QObject
{
    public:
        explicit AbstractDataHandler(QObject *parent = 0);

        virtual void asyncStart(const QUrl& url) = 0;

    signals:
        void completed(const QUrl& url);

    private:
        Q_OBJECT
};

#endif
