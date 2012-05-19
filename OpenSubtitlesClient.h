
#ifndef OpenSubtitlesClient_h
#define OpenSubtitlesClient_h

#include "maiaXmlRpcClient.h"
#include <cstdint>



class OpenSubtitlesClient : public QObject
{
    public:
        OpenSubtitlesClient(QObject *parent = 0);

        bool logged() const;

    public slots:
        void login();
        void logout();
        void search(const QString& filename);

    private slots:
        void handleLogin(QVariant &);
        void handleResponse(QVariant &);
        void handleError(int error, const QString &message);
        void handleSslErrors(QNetworkReply *, const QList<QSslError> &);

    protected:
        uint64_t computeHash(const QString& filename);

    private:
        Q_OBJECT

        MaiaXmlRpcClient *rpc;

        QString token;
};

#endif
