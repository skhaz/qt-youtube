#include <QThread>
#include <QMutexLocker>
#include "Logger.h"

class LoggerThread : public QThread
{
    enum ThreadState
    {
        STATE_RUNNING,
        STATE_TERMINATED
    };

public:
    LoggerThread(Logger& parent)
        : QThread()
    {
        m_mutex = &parent.m_mutex;
        m_condition = &parent.m_condition;
        m_queue = &parent.m_queue;
        m_sink = parent.m_sink;
        m_state = STATE_RUNNING;
    }

    void run()
    {
        while(m_state != STATE_TERMINATED)
        {
            QMutexLocker locker(m_mutex);

            if(m_queue->empty())
            {
                m_condition->wait(m_mutex);
            }

            if(m_state == STATE_TERMINATED || m_queue->empty())
                continue;

            Logger::LogEntry entry = m_queue->front();
            m_queue->pop_front();

            m_sink->write(entry.logLevel, entry.logMessage);
        }
    }

    void interrupt()
    {
        m_state = STATE_TERMINATED;
    }

private:
    QMutex* m_mutex;
    QWaitCondition* m_condition;
    QQueue<Logger::LogEntry>* m_queue;
    ThreadState m_state;
    LoggerSink* m_sink;
};

Logger::Logger(LoggerSink* sink /*= nullptr*/)
    : m_level(LOG_FATAL)
{
    m_sink = sink;
    m_thread.reset(new LoggerThread(*this));
    m_thread->start();
}

Logger::~Logger()
{
    m_thread->interrupt();
    m_condition.wakeOne();
    m_thread->wait();
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
