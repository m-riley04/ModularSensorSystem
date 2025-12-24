@echo off
if /i "%VERBOSE%"=="1" (echo on) else (echo off)
ver >nul REM resets errorlevel

set "STARTTIME=%TIME%"
echo Start Time: %STARTTIME%
timeout /t 5 /nobreak >nul

echo Initializing build_deps.bat script...

REM checks
REM TODO: call tools\check_env.bat || exit /b 1
REM TODO: call tools\check_tools.bat || exit /b 1

REM initialize environment
call tools\init_env.bat || exit /b 1

REM have the user confirm before proceeding
echo The following dependencies will be built and installed to:
echo - "%DEPS_ROOT%"
echo - "%SRC_ROOT%"
set /p confirm="Proceed? (y/n): "
if /i not "%confirm%"=="y" (
  echo Aborting.
  exit /b 1
)

call tools\init_onnxruntime.bat || exit /b 1
call tools\init_gstreamer.bat || exit /b 1

set ENDTIME=%TIME%
echo End Time: %ENDTIME%
echo Duration (Approx): %ENDTIME% - %STARTTIME%

echo Done.