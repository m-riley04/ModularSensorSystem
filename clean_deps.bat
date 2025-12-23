@echo off

SETLOCAL
REM Check base environment variables
echo Checking environment variables...

SET default_dev_root="C:\dev"
IF "%DEV_ROOT%"=="" (
	echo WARNING: DEV_ROOT environment variable is not set.
	echo Defaulting to "%default_dev_root%"...
	SET DEV_ROOT="%default_dev_root%"
)

IF NOT EXIST "%DEV_ROOT%" (
	echo ERROR: DEV_ROOT directory "%DEV_ROOT%" does not exist.
	exit /b 1
)

SET default_deps_root="%default_dev_root%\deps"
IF "%DEPS_ROOT%"=="" (
	echo WARNING: DEPS_ROOT environment variable is not set.
	echo Defaulting to "%default_deps_root%"...
	SET DEPS_ROOT="%default_deps_root%"
)

IF NOT EXIST "%DEPS_ROOT%" (
	echo ERROR: DEPS_ROOT directory "%DEPS_ROOT%" does not exist.
	exit /b 1
)

REM set onnxruntime environment variables
SET ORT_SRC="%DEPS_ROOT%\onnxruntime"
SET ORT_BUILD="%ORT_SRC%\build\Windows\RelWithDebugInfo"
SET ORT_INSTALL_PREFIX="%ORT_BUILD_DIR%\install"
SET ORT_ROOT="%DEPS_ROOT%\onnxruntime"
SET ORT_PKGCFG="%ORT_ROOT%\lib\pkgconfig"

REM set GStreamer environment variables
SET GST_ROOT="%DEPS_ROOT%\gstreamer"

REM add ORT and GST to path
SET PATH="%ORT_ROOT%\bin;%GST_ROOT%\bin;%PATH%"