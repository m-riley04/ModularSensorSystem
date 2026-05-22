#include <QtTest>
#include "features/ielement.hpp"
#include "utils/element_utils.hpp"
#include "utils/plugins_utils.hpp"
#include "utils/boost_qt_conversions.hpp"

// Compiled directly to avoid linking against MSSCore.lib.
// session_utils.cpp is excluded because it transitively includes <gst/gst.h>;
// add GStreamerProps.props to this project and re-enable those tests when ready.
#include "../../core/src/utils/utils.cpp"

class UtilsTests : public QObject
{
    Q_OBJECT

private slots:

    // -------------------------------------------------------------------------
    // generateTimestampNs
    // -------------------------------------------------------------------------

    void generateTimestampNs_returnsNonZero()
    {
        QVERIFY(generateTimestampNs() > 0);
    }

    void generateTimestampNs_isMonotonicallyIncreasing()
    {
        ns t1 = generateTimestampNs();
        ns t2 = generateTimestampNs();
        QVERIFY(t2 >= t1);
    }

    void generateTimestampNs_twoCallsDifferByReasonableAmount()
    {
        // Two back-to-back calls should never be more than 1 second apart
        ns t1 = generateTimestampNs();
        ns t2 = generateTimestampNs();
        QVERIFY(t2 - t1 < 1'000'000'000ULL);
    }

    // -------------------------------------------------------------------------
    // isSharedLib
    // -------------------------------------------------------------------------

    void isSharedLib_dllIsSharedLib()
    {
        QVERIFY(isSharedLib(std::filesystem::path("MyPlugin.dll")));
    }

    void isSharedLib_exeIsNotSharedLib()
    {
        QVERIFY(!isSharedLib(std::filesystem::path("MyPlugin.exe")));
    }

    void isSharedLib_libIsNotSharedLib()
    {
        QVERIFY(!isSharedLib(std::filesystem::path("MSSCore.lib")));
    }

    void isSharedLib_noExtensionIsNotSharedLib()
    {
        QVERIFY(!isSharedLib(std::filesystem::path("MSSCore")));
    }

    // -------------------------------------------------------------------------
    // looksLikePlugin
    // -------------------------------------------------------------------------

    void looksLikePlugin_validPluginDllReturnsTrue()
    {
        QVERIFY(looksLikePlugin(std::filesystem::path("TestDataSourcePlugin.dll")));
    }

    void looksLikePlugin_dllWithoutPluginSuffixReturnsFalse()
    {
        QVERIFY(!looksLikePlugin(std::filesystem::path("MSSCore.dll")));
    }

    void looksLikePlugin_nonDllWithPluginInNameReturnsFalse()
    {
        QVERIFY(!looksLikePlugin(std::filesystem::path("TestDataSourcePlugin.exe")));
    }

    void looksLikePlugin_emptyPathReturnsFalse()
    {
        QVERIFY(!looksLikePlugin(std::filesystem::path("")));
    }

    void looksLikePlugin_tooShortNameReturnsFalse()
    {
        // Filename must be longer than MSS_PLUGIN_NAME_MIN_LENGTH (10) chars
        QVERIFY(!looksLikePlugin(std::filesystem::path("Plugin.dll")));
    }

    // -------------------------------------------------------------------------
    // elementTypeToString
    // -------------------------------------------------------------------------

    void elementTypeToString_source()
    {
        QCOMPARE(elementTypeToString(IElement::Type::Source), std::string("Source"));
    }

    void elementTypeToString_processor()
    {
        QCOMPARE(elementTypeToString(IElement::Type::Processor), std::string("Processor"));
    }

    void elementTypeToString_mount()
    {
        QCOMPARE(elementTypeToString(IElement::Type::Mount), std::string("Mount"));
    }

    void elementTypeToString_unknown()
    {
        QCOMPARE(elementTypeToString(IElement::Type::Unknown), std::string("Unknown"));
    }

    // -------------------------------------------------------------------------
    // boostUuidToQUuid / QUuidToBoostUuid — roundtrip correctness
    // -------------------------------------------------------------------------

    void uuidRoundtrip_boostToQAndBack()
    {
        boost::uuids::random_generator gen;
        boost::uuids::uuid original = gen();
        boost::uuids::uuid restored = QUuidToBoostUuid(boostUuidToQUuid(original));
        QVERIFY(original == restored);
    }

    void uuidRoundtrip_nilUuid()
    {
        boost::uuids::uuid nil{};
        boost::uuids::uuid restored = QUuidToBoostUuid(boostUuidToQUuid(nil));
        QVERIFY(restored == nil);
    }

    void uuidRoundtrip_qToBoostAndBack()
    {
        QUuid original = QUuid::createUuid();
        QCOMPARE(boostUuidToQUuid(QUuidToBoostUuid(original)), original);
    }

    void uuidConversion_stringRepresentationsMatch()
    {
        boost::uuids::random_generator gen;
        boost::uuids::uuid boostId = gen();
        QString boostStr = QString::fromStdString(boost::uuids::to_string(boostId)).toLower();
        QString qStr = boostUuidToQUuid(boostId).toString(QUuid::WithoutBraces).toLower();
        QCOMPARE(qStr, boostStr);
    }
};

#include <controllers/maincontroller.hpp>


class ControllersTests : public QObject
{
    Q_OBJECT

private:
    QSettings m_settings;
    MainController m_controller{ m_settings, nullptr };

private slots:

	void mainController_initializesElementsController()
	{
		// Just check that we can access the elements controller without crashing; more detailed tests are in ElementsControllerTests
		ElementsController& ec = m_controller.elementsController();
		QVERIFY(&ec != nullptr);
	}

};

#include <pipeline/sessionpipeline.hpp>

class SessionPipelineTests : public QObject
{
    Q_OBJECT

private:
    QSettings m_qsettings;
    MainController m_controller{ m_qsettings, nullptr };
    SessionSettings m_settings{};
    SessionPipeline m_pipeline{ m_settings, m_controller.elementsController() };

private slots:

	void sessionPipeline_initialStateIsStopped()
	{
		QCOMPARE(m_pipeline.state(), SessionPipeline::State::STOPPED);
	}

};

#include "main.moc"

static QList<QObject*> allTestObjects()
{
    return {
        new UtilsTests,
        new ControllersTests,
        new SessionPipelineTests,
    };
}

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    // Support --list_content so the Qt Test Adapter in Visual Studio can
    // discover tests without requiring debug symbols.
    for (int i = 1; i < argc; ++i) {
        if (qstrcmp(argv[i], "--list_content") == 0) {
            for (QObject* obj : allTestObjects()) {
                const QMetaObject* meta = obj->metaObject();
                printf("%s\n", meta->className());
                for (int m = 0; m < meta->methodCount(); ++m) {
                    QMetaMethod method = meta->method(m);
                    if (method.methodType() == QMetaMethod::Slot &&
                        method.access() == QMetaMethod::Private) {
                        printf("  %s\n", method.name().constData());
                    }
                }
                delete obj;
            }
            return 0;
        }
    }

    int status = 0;
    for (QObject* obj : allTestObjects())
        status |= QTest::qExec(obj, argc, argv);

    return status;
}
