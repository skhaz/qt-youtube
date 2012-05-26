#ifndef LOGFILESINK_H
#define LOGFILESINK_H

#include <QFile>
#include "Logger.h"

/// The LogFileSink is a sink class concept that outputs everything what the
/// Logger writes, to a file.
class LogFileSink : public LoggerSink
{
public:
    /// Creates the LogFileSink.
    LogFileSink(QFile& file);

    /// Destroys the LogFileSink.
    virtual ~LogFileSink();

    /// Write the log level and the log message in a formatted way.
    /// The current format is: DD/MM/YYYY - hh:mm:ss [level] : message
    void write(LoggerLevel level, const QString &message);

    /// Returns the given log level as a string.
    const QString& loggerLevelToString(LoggerLevel level) const;

private:
    /// Log file.
    QFile& m_file;
};

#endif // LOGFILESINK_H
