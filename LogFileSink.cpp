#include <QTextStream>
#include <QDateTime>
#include "LogFileSink.h"

LogFileSink::LogFileSink(QFile& file)
    : m_file(file)
{

}

LogFileSink::~LogFileSink()
{

}

void LogFileSink::write(LoggerLevel level, const QString &message)
{
    QTextStream out(&m_file);
    QDateTime dateTime = QDateTime::currentDateTime();
    QString logMessage =
        QString("%1 [%2] : %3")
        .arg(dateTime.toString("dd/MM/yyyy - hh:mm:ss"))
        .arg(loggerLevelToString(level))
        .arg(message);
    out << logMessage << endl;
}

const QString& LogFileSink::loggerLevelToString(LoggerLevel level) const
{
    static const QString LEVELS[] =
    {
        "FATAL", // LOG_FATAL
        "CRITICAL", // LOG_CRITICAL
        "ERROR", // LOG_ERROR
        "WARNING", // LOG_WARNING
        "INFORMATION", // LOG_INFORMATION
        "DEBUG" // LOG_DEBUG
    };

    return LEVELS[level];
}
