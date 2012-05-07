
#include "OpenSubtitlesClient.h"



OpenSubtitlesClient::OpenSubtitlesClient(QObject* parent) : QObject(parent)
{
    rpc = new MaiaXmlRpcClient(QUrl("http://api.opensubtitles.org/xml-rpc"), this);

    QSslConfiguration config = rpc->sslConfiguration();
    config.setProtocol(QSsl::AnyProtocol);
    rpc->setSslConfiguration(config);

    connect(rpc, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
            this, SLOT(handleSslErrors(QNetworkReply *, const QList<QSslError> &)));

    login();
}

bool OpenSubtitlesClient::logged() const
{
    return !token.isEmpty();
}

void OpenSubtitlesClient::login()
{
    QVariantList args;
    args << "" << "" << "pt" << "OS Test User Agent";

    rpc->call("LogIn", args,
              this, SLOT(handleLogin(QVariant &)),
              this, SLOT(handleError(int, const QString &)));
}

void OpenSubtitlesClient::logout()
{
}

void OpenSubtitlesClient::search(const QString& filename)
{
    QString hash = QString::number(computeHash(filename), 16);
    QFile file(filename);
    file.open(QFile::ReadOnly);

    QVariantList args;
    QVariantMap params;
    QVariantMap data;

    data["token"] = token;
    data["moviehash"] =  hash;
    data["moviebytesize"] = QString::number(file.size());
    params[""] = data;

    args << token << params;

    rpc->call("SearchSubtitles", args,
              this, SLOT(handleResponse(QVariant &)),
              this, SLOT(handleError(int, const QString &)));
}

void OpenSubtitlesClient::handleLogin(QVariant& result)
{
    QVariantMap resultMap = result.toMap();

    if (resultMap["status"].toString() == QLatin1String("200 OK")) {
        token = resultMap["token"].toString();

        // XXX
        search("film.mkv");
    }
}

void OpenSubtitlesClient::handleResponse(QVariant& result)
{
    qDebug() << result.toMap();
}

void OpenSubtitlesClient::handleError(int error, const QString& message)
{
    qDebug() << error << "-" << message;
}

void OpenSubtitlesClient::handleSslErrors(QNetworkReply *reply, const QList<QSslError>& errors)
{
    qDebug() << "SSL Error:" << errors;
    reply->ignoreSslErrors(); // FIX
}

uint64_t OpenSubtitlesClient::computeHash(const QString& filename)
{
    // http://trac.opensubtitles.org/projects/opensubtitles/wiki/HashSourceCodes
    FILE* handle = fopen(filename.toLocal8Bit(), "rb");

    if (!handle) {
        return -1;
    }

    uint64_t hash, size;

    fseek(handle, 0, SEEK_END);
    hash = size = ftell(handle);
    fseek(handle, 0, SEEK_SET);

    for(uint64_t tmp = 0, i = 0; i < 65536/sizeof(tmp) && fread((char*)&tmp, sizeof(tmp), 1, handle); hash += tmp, i++);
    fseek(handle, std::max((uint64_t)0, size - 65536), SEEK_SET);
    for(uint64_t tmp = 0, i = 0; i < 65536/sizeof(tmp) && fread((char*)&tmp, sizeof(tmp), 1, handle); hash += tmp, i++);

    return hash;
}
