#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QStringList>
#include <QMutex>
#include <QWaitCondition>
#include <QThread>
#include <QQueue>

/// The level of the Logger or the log message.
enum LoggerLevel
{
    LOG_FATAL,
    LOG_CRITICAL,
    LOG_ERROR,
    LOG_WARNING,
    LOG_INFORMATION,
    LOG_DEBUG
};

// Forward declaration
class LoggerThread;

/// The LoggerSink class is an interface for a Sink concept. Each time the
/// logger receives a log message, it is passed to the LoggerSink, allowing it
/// to be outputted to a terminal, file or what the user desire.
class LoggerSink
{
public:
    virtual ~LoggerSink(){}

    /// Receives the log level and log message.
    virtual void write(LoggerLevel level, const QString& message) = 0;
};

/// The Logger class provides a simple logging engine.
class Logger : public QThread
{
public:
    /// Creates the Logger.
    Logger(LoggerSink* sink = nullptr);

    /// Detroys the Logger.
    ~Logger();

    /// Set the logger level.
    void setLevel(LoggerLevel level);

    /// Get the current logging level.
    LoggerLevel getLevel() const;

    /// Set the logger sink.
    void setSink(LoggerSink& sink);

    /// Get the logger sink.
    LoggerSink& getSink();

    /// Logs the given message. If the level of the logger is the same or lower
    /// than the one passed by argument the message will not be discarded.
    void log(LoggerLevel level, const QString& message);

private:
    /// Worker thread. Process the messages and writes them to the sink.
    void run();

private:
    /// Internal thread state
    enum ThreadState
    {
        STATE_RUNNING,
        STATE_TERMINATED
    };

    /// Internal log entry.
    struct LogEntry
    {
        LoggerLevel logLevel;
        QString logMessage;
    };

    /// Logger level.
    LoggerLevel m_level;

    /// Queue mutex.
    QMutex m_mutex;

    /// Queue emptynesss notifier.
    QWaitCondition m_condition;

    /// Log queue.
    QQueue<LogEntry> m_queue;

    /// Log sink.
    LoggerSink* m_sink;

    /// Thread state.
    ThreadState m_threadState;

    friend class LoggerThread;
};

#endif // LOGGER_H
