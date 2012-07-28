#ifndef APPLICATION_H
#define APPLICATION_H

#include <QScopedPointer>
#include <QApplication>
#include <QFile>
#include <string>
#include <vector>
#include "Logger.h"
#include "LogFileSink.h"

/// The Application class is responsible for further initialization of the app
/// and provides acessors to the current instance and internal resources. It
/// also take cares of shutdown cleanup. An Application class must be
/// instantiaded only once.
class Application
{
public:
    /// Creates the Application.
    Application(int argc, char** argv);

    /// Returns a pointer to the current Application instance;
    static Application* instance();

    /// This is the equivalent of the main function.
    void main(const std::vector<std::string>& arguments);

    /// Initialize the Application and process the command line arguments.
    void initialize();

    /// Runs the Application and returns the exit code.
    int run();

    /// Returns the application logger.
    Logger& logger();

    /// Return the arguments that should be passed during the initialization
    /// of a libvlc object;
    const std::vector<std::string>& vlcArguments() const;

private:
    /// Command line arguments
    std::vector<std::string> m_arguments;

    /// Vlc arguments
    std::vector<std::string> m_vlc_arguments;

    /// Current log file.
    QFile m_logFile;

    /// Current log sink.
    LogFileSink m_logSink;

    /// Logger instance
    Logger m_logger;

    /// Qt application
    QApplication m_application;

    /// Current application instance
    static Application* m_instance;
};

/// ENTRY_POINT is a macro that implements the main function.
#define ENTRY_POINT \
    int main(int argc, char** argv) \
    { \
        QScopedPointer<Application> app(new Application(argc, argv)); \
        try \
        { \
            app->initialize(); \
        } \
        catch(const std::exception& ex) \
        { \
            app->logger().log(LOG_CRITICAL, "Could not initialize the application."); \
        } \
        catch(...) \
        { \
            app->logger().log(LOG_FATAL, "A fatal error ocurred."); \
        } \
        return app->run(); \
    }

#endif // APPLICATION_H
