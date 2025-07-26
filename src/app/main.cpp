#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDir>

int main(int argc, char* argv[])
{
#if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Add QML import paths
    engine.addImportPath("qrc:/qt/qml/app/");
    engine.addImportPath("qrc:/qt/qml/app/app/");
    engine.addImportPath("qrc:/qt/qml/core/");
    engine.addImportPath("qrc:/qt/qml/src/");
    engine.addImportPath("qrc:/qt/qml/src/app/");
    engine.addImportPath("qrc:/qt/qml/src/app/app/");

    engine.load(QUrl("qrc:/qt/qml/app/main.qml"));
    if (engine.rootObjects().isEmpty()) {
        qWarning() << "Failed to load QML root object (is empty).";
        qWarning() << "Available import paths:";
        for (const QString &path : engine.importPathList()) {
            qWarning() << "  -" << path;
        }
        return EXIT_FAILURE;
    }

    return app.exec();
}
