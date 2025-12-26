@echo off
if /i "%VERBOSE%"=="1" (echo on) else (echo off)

:onnxruntime
echo === onnxruntime ===

:onnxruntime_fetch
echo Fetching source code...
IF NOT EXIST "%ORT_SRC%" (
	echo Cloning...
	git clone --recursive https://github.com/microsoft/onnxruntime.git "%ORT_SRC%" || exit /b 1
)
pushd "%ORT_SRC%" || exit /b 1
set "ort_version=1.23.0"
git fetch --tags || exit /b 1
git checkout -B v"%ort_version%" "refs/tags/v%ort_version%" || exit /b 1

:onnxruntime_build
echo Building with build.bat...
REM check if already built
if exist "%ORT_BUILD%" (
  echo onnxruntime already built, skipping build step.
  goto onnxruntime_install
)

REM TODO: make these options configurable
call ".\build.bat" ^
  --config RelWithDebInfo ^
  --build_shared_lib ^
  --parallel ^
  --compile_no_warning_as_error ^
  --minimal_build ^
  --skip_submodule_sync ^
  --skip_tests
REM check for error message
set "rc=%errorlevel%"

if "%rc%"=="0" (
  echo onnxruntime build succeeded.
) else if "%rc%"=="8" (
  echo WARNING: onnxruntime tests failed (ctest exit code 8). Continuing anyway.
) else (
  echo ERROR: onnxruntime build failed (exit code %rc%).
  exit /b %rc%
)

:onnxruntime_install
echo Building and installing with CMake...
cmake -S "%ORT_SRC%\cmake" -B "%ORT_BUILD%" -DCMAKE_INSTALL_PREFIX="%ORT_INSTALL_PREFIX%" || exit /b 1
cmake --install "%ORT_BUILD%" --config RelWithDebInfo || exit /b 1

REM add pkg-config file for libonnxruntime
if not exist "%ORT_PKGCFG%" (
  mkdir "%ORT_PKGCFG%" || exit /b 1
)
(
  echo prefix=%ORT_ROOT%
  echo exec_prefix=${prefix}
  echo libdir=${exec_prefix}/lib
  echo includedir=${prefix}/include/onnxruntime
  echo
  echo Name: libonnxruntime
  echo Description: ONNX Runtime library
  echo Version: %ort_version%
  echo Libs: -L${libdir} -lonnxruntime
  echo Cflags: -I${includedir}
) > "%ORT_PKGCFG%\libonnxruntime.pc"

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

echo Validation complete.

echo onnxruntime initialization complete.

popd
