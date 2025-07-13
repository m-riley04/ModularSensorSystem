#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <core/controllers/MainController/maincontroller.h>
#include <qqmlcontext.h>

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    auto pController = std::make_unique<MainController>();

    qmlRegisterSingletonInstance("MSS.Controller", 1, 0, "MainController", pController.get());

    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/mssfrontendqml/main.qml")));
    if (engine.rootObjects().isEmpty()) {
		qWarning() << "Failed to load QML root object (is empty).";
        return EXIT_FAILURE;
    }

	

    return app.exec();
}
