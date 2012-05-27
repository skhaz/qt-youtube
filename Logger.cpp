#include <QThread>
#include <QMutexLocker>
#include "Logger.h"

Logger::Logger(LoggerSink* sink /*= nullptr*/)
{
    m_level = LOG_FATAL;
    m_sink = sink;
    m_threadState = STATE_RUNNING;
    start();
}

Logger::~Logger()
{
    // Set the logger thread as terminated
    m_threadState = STATE_TERMINATED;

    // Notify the thread
    m_condition.wakeOne();

    // Wait for termination
    wait();
}

void Logger::setLevel(LoggerLevel level)
{
    m_level = level;
}

LoggerLevel Logger::getLevel() const
{
    return m_level;
}

void Logger::setSink(LoggerSink& sink)
{
    m_sink = &sink;
}

LoggerSink& Logger::getSink()
{
    return *m_sink;
}

void Logger::log(LoggerLevel level, const QString& message)
{
    QMutexLocker locker(&m_mutex);
    bool notify = m_queue.empty();

    if(level <= m_level)
    {
        LogEntry entry =
        {
            level,
            message
        };
        m_queue.push_back(entry);

        if(notify)
            m_condition.wakeOne();
    }
}

void Logger::run()
{
    while(m_threadState != STATE_TERMINATED)
    {
        QMutexLocker locker(&m_mutex);

        if(m_queue.empty())
        {
            m_condition.wait(&m_mutex);
        }

        if(m_threadState == STATE_TERMINATED || m_queue.empty())
            continue;

        Logger::LogEntry entry = m_queue.front();
        m_queue.pop_front();

        m_sink->write(entry.logLevel, entry.logMessage);
    }
}
