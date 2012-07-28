#include <QApplication>
#include <QtGui>
#include <QtDeclarative>
#include <QGLWidget>
#include <QFile>
#include "Application.h"
#include "Player.h"
#include "Media.h"
#include "YouTubeSearch.h"

// Initializes the Application instance as null
Application* Application::m_instance = nullptr;

Application::Application(int argc, char** argv)
    : m_application(argc, argv)
    , m_logSink(m_logFile)
    , m_logger(&m_logSink)
{
    m_instance = this;
    m_arguments.assign(argv, argv + argc);
    m_logger.setLevel(LOG_DEBUG);
}

Application* Application::instance()
{
    return m_instance;
}

void Application::main(const std::vector<std::string>& arguments)
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    qmlRegisterType<Player>("OmniMedia", 1, 0, "Player");
    qmlRegisterType<Media>("OmniMedia", 1, 0, "Media");

    QDeclarativeView* view = new QDeclarativeView;

    YouTubeSearch *youtube = new YouTubeSearch(view);
    youtube->setContext(view->rootContext());

    QObject::connect(view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view->setAttribute(Qt::WA_OpaquePaintEvent);
    view->setAttribute(Qt::WA_NoSystemBackground);
    view->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    view->viewport()->setAttribute(Qt::WA_NoSystemBackground);
    view->setWindowTitle("OmniMedia");
    view->setSource(QUrl::fromLocalFile("Main.qml"));

    QObject *root = qobject_cast<QObject *>(view->rootObject());
    QObject *searchInput = root->findChild<QObject *>("searchInput");

    if (searchInput)
        QObject::connect(searchInput, SIGNAL(queryChanged(QString)), youtube, SLOT(search(QString)));

    view->show();
}

void Application::initialize()
{
    m_logFile.setFileName("log.txt");
    m_logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    logger().log(LOG_DEBUG, "Initializing application.");

    // Copy the necessary arguments for vlc working properly
    logger().log(LOG_DEBUG, "Parsing VLC arguments.");
    static const char* libVlcArguments[] =
    {
        "--intf=dummy",
        "--no-video-title-show",
        "--no-stats",
        "--no-snapshot-preview",
        "--quiet"
    };
    std::copy(std::begin(libVlcArguments),
              std::end(libVlcArguments),
              std::back_inserter(m_vlc_arguments));
}

int Application::run()
{
    int returnCode = EXIT_FAILURE;
    try
    {
        logger().log(LOG_INFORMATION, "Starting application.");

        // Call the main function
        main(m_arguments);

        returnCode = m_application.exec();
    }
    catch(const std::exception& ex)
    {
        logger().log(LOG_ERROR, ex.what());
    }
    catch(...)
    {
        logger().log(LOG_CRITICAL, "An unhandled exception has been thrown.");
    }

    logger().log(LOG_DEBUG,
        QString("Application terminated with return code %1.").arg(returnCode));

    return returnCode;
}

Logger& Application::logger()
{
    return m_logger;
}

const std::vector<std::string>& Application::vlcArguments() const
{
    return m_vlc_arguments;
}

// Implements the main function here.
ENTRY_POINT
