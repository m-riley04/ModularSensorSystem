@echo off
if /i "%VERBOSE%"=="1" (echo on) else (echo off)

echo Initializing build environment...

REM reset
SET "DEPS_ROOT="
SET "SRC_ROOT="

REM check for required environment variables
SET "default_deps_root=%CD%\deps"
SET "DEPS_ROOT=%default_deps_root%"

IF NOT EXIST "%DEPS_ROOT%" (
	echo WARNING: DEPS_ROOT directory "%DEPS_ROOT%" does not exist.
    echo Creating...
    MKDIR "%DEPS_ROOT%" || exit /b 1
)

SET "default_srcs_root=%default_deps_root%\srcs"
SET "SRC_ROOT=%default_srcs_root%"

IF NOT EXIST "%SRC_ROOT%" (
	echo WARNING: SRC_ROOT directory "%SRC_ROOT%" does not exist.
    echo Creating...
    MKDIR "%SRC_ROOT%" || exit /b 1
)

REM set onnxruntime environment variables
SET "ORT_SRC=%SRC_ROOT%\onnxruntime"
SET "ORT_ROOT=%DEPS_ROOT%\onnxruntime"
SET "ORT_BUILD=%ORT_SRC%\build\Windows\RelWithDebInfo"
SET "ORT_INSTALL_PREFIX=%ORT_ROOT%"
SET "ORT_PKGCFG=%ORT_ROOT%\lib\pkgconfig"

SET "GST_SRC=%SRC_ROOT%\gstreamer"
SET "GST_ROOT=%DEPS_ROOT%\gstreamer"

REM add ORT and GST to path
SET "PATH=%ORT_ROOT%\bin;%GST_ROOT%\bin;%PATH%"
SET "PKG_CONFIG_PATH=%ORT_PKGCFG%;%PKG_CONFIG_PATH%"

echo Environment initialized.
