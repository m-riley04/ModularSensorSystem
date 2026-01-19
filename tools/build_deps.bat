@echo off
if /i "%VERBOSE%"=="1" (echo on) else (echo off)
ver >nul REM resets errorlevel

REM start stopwatch
set "startTime=%time: =0%"

echo Initializing build_deps.bat script...

REM checks
REM TODO: call tools\check_env.bat || exit /b 1
REM TODO: call tools\check_tools.bat || exit /b 1

REM initialize environment
call "%~dp0init_env.bat" || exit /b 1

REM have the user confirm before proceeding
echo The following dependencies will be built and installed to:
echo - "%DEPS_ROOT%"
echo - "%SRC_ROOT%"
set /p confirm="Proceed? (y/n): "
if /i not "%confirm%"=="y" (
  echo Aborting.
  exit /b 1
)

call "%~dp0init_onnxruntime.bat" || exit /b 1
call "%~dp0init_gstreamer.bat" || exit /b 1

REM copy required files to where they need to be

echo Done.

REM stop stopwatch
set "endTime=%time: =0%"
call "%~dp0time_utils.bat" :GetElapsed "%startTime%" "%endTime%" elapsed

REM print details
echo --- Run details ---
echo Start:    %startTime%
echo End:      %endTime%
echo Elapsed:  %elapsed%
  
