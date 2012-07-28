#ifndef OPENSUBTITLESCLIENT_H
#define OPENSUBTITLESCLIENT_H

#include "maiaXmlRpcClient.h"


class OpenSubtitlesClient : public QObject
{
    Q_OBJECT

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
    MaiaXmlRpcClient *rpc;
    QString token;
};

#endif // OPENSUBTITLESCLIENT_H
