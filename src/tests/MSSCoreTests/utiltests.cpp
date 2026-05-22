#include "utiltests.hpp"

#include "features/ielement.hpp"
#include "utils/element_utils.hpp"
#include "utils/plugins_utils.hpp"
#include "utils/boost_qt_conversions.hpp"

// NOTE: This NEEDS to be imported like this.
// The others above are fine, but when I import this one as hpp (even after adding the MSS_CORE_API export), it breaks the test discovery
// for whatever reason
#include "../../core/src/utils/utils.cpp"

void UtilTests::generateTimestampNs_returnsNonZero()
{
    QVERIFY(generateTimestampNs() > 0);
}

void UtilTests::generateTimestampNs_isMonotonicallyIncreasing()
{
    ns t1 = generateTimestampNs();
    ns t2 = generateTimestampNs();
    QVERIFY(t2 >= t1);
}

void UtilTests::generateTimestampNs_twoCallsDifferByReasonableAmount()
{
    // Two back-to-back calls should never be more than 1 second apart
    ns t1 = generateTimestampNs();
    ns t2 = generateTimestampNs();
    QVERIFY(t2 - t1 < 1'000'000'000ULL);
}

// -------------------------------------------------------------------------
// isSharedLib
// -------------------------------------------------------------------------

void UtilTests::isSharedLib_dllIsSharedLib()
{
    QVERIFY(isSharedLib(std::filesystem::path("MyPlugin.dll")));
}

void UtilTests::isSharedLib_exeIsNotSharedLib()
{
    QVERIFY(!isSharedLib(std::filesystem::path("MyPlugin.exe")));
}

void UtilTests::isSharedLib_libIsNotSharedLib()
{
    QVERIFY(!isSharedLib(std::filesystem::path("MSSCore.lib")));
}

void UtilTests::isSharedLib_noExtensionIsNotSharedLib()
{
    QVERIFY(!isSharedLib(std::filesystem::path("MSSCore")));
}

// -------------------------------------------------------------------------
// looksLikePlugin
// -------------------------------------------------------------------------

void UtilTests::looksLikePlugin_validPluginDllReturnsTrue()
{
    QVERIFY(looksLikePlugin(std::filesystem::path("TestDataSourcePlugin.dll")));
}

void UtilTests::looksLikePlugin_dllWithoutPluginSuffixReturnsFalse()
{
    QVERIFY(!looksLikePlugin(std::filesystem::path("MSSCore.dll")));
}

void UtilTests::looksLikePlugin_nonDllWithPluginInNameReturnsFalse()
{
    QVERIFY(!looksLikePlugin(std::filesystem::path("TestDataSourcePlugin.exe")));
}

void UtilTests::looksLikePlugin_emptyPathReturnsFalse()
{
    QVERIFY(!looksLikePlugin(std::filesystem::path("")));
}

void UtilTests::looksLikePlugin_tooShortNameReturnsFalse()
{
    // Filename must be longer than MSS_PLUGIN_NAME_MIN_LENGTH (10) chars
    QVERIFY(!looksLikePlugin(std::filesystem::path("Plugin.dll")));
}

// -------------------------------------------------------------------------
// elementTypeToString
// -------------------------------------------------------------------------

void UtilTests::elementTypeToString_source()
{
    QCOMPARE(elementTypeToString(IElement::Type::Source), std::string("Source"));
}

void UtilTests::elementTypeToString_processor()
{
    QCOMPARE(elementTypeToString(IElement::Type::Processor), std::string("Processor"));
}

void UtilTests::elementTypeToString_mount()
{
    QCOMPARE(elementTypeToString(IElement::Type::Mount), std::string("Mount"));
}

void UtilTests::elementTypeToString_unknown()
{
    QCOMPARE(elementTypeToString(IElement::Type::Unknown), std::string("Unknown"));
}

// -------------------------------------------------------------------------
// boostUuidToQUuid / QUuidToBoostUuid — roundtrip correctness
// -------------------------------------------------------------------------

void UtilTests::uuidRoundtrip_boostToQAndBack()
{
    boost::uuids::random_generator gen;
    boost::uuids::uuid original = gen();
    boost::uuids::uuid restored = QUuidToBoostUuid(boostUuidToQUuid(original));
    QVERIFY(original == restored);
}

void UtilTests::uuidRoundtrip_nilUuid()
{
    boost::uuids::uuid nil{};
    boost::uuids::uuid restored = QUuidToBoostUuid(boostUuidToQUuid(nil));
    QVERIFY(restored == nil);
}

void UtilTests::uuidRoundtrip_qToBoostAndBack()
{
    QUuid original = QUuid::createUuid();
    QCOMPARE(boostUuidToQUuid(QUuidToBoostUuid(original)), original);
}

void UtilTests::uuidConversion_stringRepresentationsMatch()
{
    boost::uuids::random_generator gen;
    boost::uuids::uuid boostId = gen();
    QString boostStr = QString::fromStdString(boost::uuids::to_string(boostId)).toLower();
    QString qStr = boostUuidToQUuid(boostId).toString(QUuid::WithoutBraces).toLower();
    QCOMPARE(qStr, boostStr);
}
