#pragma once

#include "testsuite.hpp"
#include <QtTest>

class UtilTests : public TestSuite
{
    Q_OBJECT

public:
    using TestSuite::TestSuite;

private slots:

    // -------------------------------------------------------------------------
    // generateTimestampNs
    // -------------------------------------------------------------------------

    void generateTimestampNs_returnsNonZero();
    void generateTimestampNs_isMonotonicallyIncreasing();
    void generateTimestampNs_twoCallsDifferByReasonableAmount();

    // -------------------------------------------------------------------------
    // isSharedLib
    // -------------------------------------------------------------------------

    void isSharedLib_dllIsSharedLib();
    void isSharedLib_exeIsNotSharedLib();
    void isSharedLib_libIsNotSharedLib();
    void isSharedLib_noExtensionIsNotSharedLib();

    // -------------------------------------------------------------------------
    // looksLikePlugin
    // -------------------------------------------------------------------------

    void looksLikePlugin_validPluginDllReturnsTrue();
    void looksLikePlugin_dllWithoutPluginSuffixReturnsFalse();
    void looksLikePlugin_nonDllWithPluginInNameReturnsFalse();
    void looksLikePlugin_emptyPathReturnsFalse();
    void looksLikePlugin_tooShortNameReturnsFalse();

    // -------------------------------------------------------------------------
    // elementTypeToString
    // -------------------------------------------------------------------------

    void elementTypeToString_source();
    void elementTypeToString_processor();
    void elementTypeToString_mount();
    void elementTypeToString_unknown();

    // -------------------------------------------------------------------------
    // boostUuidToQUuid / QUuidToBoostUuid — roundtrip correctness
    // -------------------------------------------------------------------------

    void uuidRoundtrip_boostToQAndBack();
    void uuidRoundtrip_nilUuid();
    void uuidRoundtrip_qToBoostAndBack();
    void uuidConversion_stringRepresentationsMatch();

    /// tests

    void testTest();

};
