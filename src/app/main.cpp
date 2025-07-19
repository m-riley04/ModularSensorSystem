#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqmlcontext.h>

int main(int argc, char* argv[])
{
#if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/App/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        qWarning() << "Failed to load QML root object (is empty).";
        return EXIT_FAILURE;
    }

    return app.exec();
}
