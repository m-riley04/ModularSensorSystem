@echo off
if /i "%VERBOSE%"=="1" (echo on) else (echo off)

echo Initializing build_deps.bat script...

REM check if already built
IF "%DEPS_BUILT%" == "1" (
	echo Dependencies already built.
	echo (If this is not the case, please unset the DEPS_BUILT environment variable and try again.)
	goto :EOF
)

REM Check tools --- must be running in a Developer Command Prompt (or vcvars already called)
where cl.exe >nul 2>&1
if errorlevel 1 (
  echo ERROR: MSVC compiler (cl.exe) not found on PATH.
  echo        Launch this script from "x64 Native Tools Command Prompt for VS"
  echo        or call vcvars64.bat before running.
  exit /b 1
)

where link.exe >nul 2>&1
if errorlevel 1 (
  echo ERROR: MSVC linker (link.exe) not found on PATH.
  echo        Launch from VS Developer Prompt (x64) or call vcvars64.bat.
  exit /b 1
)

REM check architecture
echo %VSCMD_ARG_TGT_ARCH% | findstr /i "x64" >nul
if errorlevel 1 (
  echo WARNING: VSCMD_ARG_TGT_ARCH is not x64 (current: "%VSCMD_ARG_TGT_ARCH%").
  echo          You may not be in an x64 VS Developer Prompt.
)

REM --- basic build tooling
where git.exe >nul 2>&1
if errorlevel 1 (
  echo ERROR: git.exe not found on PATH.
  exit /b 1
)

where cmake.exe >nul 2>&1
if errorlevel 1 (
  echo ERROR: cmake.exe not found on PATH.
  exit /b 1
)

where python.exe >nul 2>&1
if errorlevel 1 (
  echo ERROR: python.exe not found on PATH.
  echo        Meson typically requires Python.
  exit /b 1
)

where meson.exe >nul 2>&1
if errorlevel 1 (
  echo ERROR: meson.exe not found on PATH.
  echo        Install with: pip install meson
  exit /b 1
)

where ninja.exe >nul 2>&1
if errorlevel 1 (
  echo ERROR: ninja.exe not found on PATH.
  echo        Install with: pip install ninja  (or install Ninja separately)
  exit /b 1
)

where pkg-config.exe >nul 2>&1
if errorlevel 1 (
  echo ERROR: pkg-config.exe not found on PATH.
  echo        GStreamer builds frequently require pkg-config.
  echo        If using MSYS2, ensure pkg-config is installed and on PATH.
  exit /b 1
)

SETLOCAL
REM Check base environment variables
echo Checking environment variables...

SET "default_dev_root=C:\dev"
IF NOT DEFINED DEV_ROOT (
	echo WARNING: DEV_ROOT environment variable is not set.
	echo Defaulting to "%default_dev_root%"...
	SET "DEV_ROOT=%default_dev_root%"
)

IF NOT EXIST "%DEV_ROOT%" (
	echo ERROR: DEV_ROOT directory "%DEV_ROOT%" does not exist.
	exit /b 1
)

SET "default_deps_root=%default_dev_root%\deps"
IF NOT DEFINED DEPS_ROOT (
	echo WARNING: DEPS_ROOT environment variable is not set.
	echo Defaulting to "%default_deps_root%"...
	SET "DEPS_ROOT=%default_deps_root%"
)

IF NOT EXIST "%DEPS_ROOT%" (
	echo ERROR: DEPS_ROOT directory "%DEPS_ROOT%" does not exist.
	exit /b 1
)

REM set onnxruntime environment variables
SET "ORT_SRC=%DEV_ROOT%\onnxruntime"
SET "ORT_ROOT=%DEPS_ROOT%\onnxruntime"
SET "ORT_BUILD=%ORT_SRC%\build\Windows\RelWithDebInfo"
SET "ORT_INSTALL_PREFIX=%ORT_ROOT%"
SET "ORT_PKGCFG=%ORT_ROOT%\lib\pkgconfig"

SET "GST_SRC=%DEV_ROOT%\gstreamer"
SET "GST_ROOT=%DEPS_ROOT%\gstreamer"

REM add ORT and GST to path
SET "PATH=%ORT_ROOT%\bin;%GST_ROOT%\bin;%PATH%"
SET "PKG_CONFIG_PATH=%ORT_PKGCFG%;%PKG_CONFIG_PATH%"

REM -------------- ORT --------------
:onnxruntime
echo === onnxruntime ===
echo Initializing...

:onnxruntime_fetch
echo Fetching source code...
IF NOT EXIST "%ORT_SRC%" (
	echo Cloning...
	git clone --recursive https://github.com/microsoft/onnxruntime.git "%ORT_SRC%" || exit /b 1
)
pushd "%ORT_SRC%" || exit /b 1
set "ort_version=v1.23.0"
git fetch --tags || exit /b 1
git checkout -B "%ort_version%" "refs/tags/%ort_version%" || exit /b 1

:onnxruntime_build
echo Building with build.bat...
call .\build.bat --config RelWithDebInfo --build_shared_lib --parallel --compile_no_warning_as_error --skip_submodule_sync || exit /b 1

:onnxruntime_install
echo Building and installing with CMake...
cmake -S "%ORT_SRC%\cmake" -B "%ORT_BUILD%" -DCMAKE_INSTALL_PREFIX="%ORT_INSTALL_PREFIX%" || exit /b 1
cmake --install "%ORT_BUILD%" --config RelWithDebInfo || exit /b 1

REM test to see if can see ORT in pkg-config
echo Validating installation...
pkg-config --version >nul 2>&1 || (echo ERROR: pkg-config not runnable. & exit /b 1)
pkg-config --exists libonnxruntime
if errorlevel 1 (
  echo ERROR: pkg-config cannot find libonnxruntime.pc
  echo        Expected PKG_CONFIG_PATH to include: %ORT_PKGCFG%
  echo        Current PKG_CONFIG_PATH: %PKG_CONFIG_PATH%
  exit /b 1
)

popd

REM -------------- GStreamer --------------
:gstreamer
echo === GStreamer ===
echo Initializing...

:gstreamer_fetch
echo Fetching source code...
IF NOT EXIST "%GST_SRC%" (
	echo Cloning...
	git clone --recursive https://gitlab.freedesktop.org/gstreamer/gstreamer.git "%GST_SRC%" || exit /b 1
)
pushd "%GST_SRC%" || exit /b 1
set "gst_version=1.27.50"
set "gst_version_short=1.27"
git fetch --tags || exit /b 1
git checkout "%gst_version%" || exit /b 1

:gstreamer_setup
echo Setting up with Meson...
set "meson_build_dir=builddir"
set "INCLUDE=%ORT_ROOT%\include\onnxruntime;%INCLUDE%"
set "PKG_CONFIG_PATH=%ORT_PKGCFG%;%PKG_CONFIG_PATH%"
if exist "%meson_build_dir%" rmdir /s /q "%meson_build_dir%"
meson setup "%meson_build_dir%" ^
  --prefix "%GST_ROOT%" ^
  --buildtype=debugoptimized ^
  -Dpkg_config_path="%ORT_PKGCFG%" ^
  -Dintrospection=disabled ^
  -Dgpl=enabled ^
  -Dgst-plugins-bad:onnx=enabled ^
  -Dgst-plugins-bad:tensordecoders=enabled ^
  -Dgst-plugins-bad:analyticsoverlay=enabled ^
  -Dgst-plugins-ugly:x264=enabled

:gstreamer_build
echo Compiling with Meson...
meson compile -C "%meson_build_dir%" || exit /b 1

:gstreamer_install
echo Installing with Meson...
meson install -C "%meson_build_dir%" || exit /b 1

popd

ENDLOCAL
set "DEPS_BUILT=1"
echo Done.