@echo off
if /i "%VERBOSE%"=="1" (echo on) else (echo off)
ver >nul REM resets errorlevel

REM start stopwatch
set "startTime=%time: =0%"
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

echo Done.

rem Get elapsed time (from: https://stackoverflow.com/questions/9922498/calculate-time-difference-in-windows-batch-file)
set "endTime=%time: =0%"
set "end=!endTime:%time:~8,1%=%%100)*100+1!"  &  set "start=!startTime:%time:~8,1%=%%100)*100+1!"
set /A "elap=((((10!end:%time:~2,1%=%%100)*60+1!%%100)-((((10!start:%time:~2,1%=%%100)*60+1!%%100), elap-=(elap>>31)*24*60*60*100"

rem Convert elapsed time to HH:MM:SS:CC format:
set /A "cc=elap%%100+100,elap/=100,ss=elap%%60+100,elap/=60,mm=elap%%60+100,hh=elap/60+100"

REM print details
echo --- Run details ---
echo Start:    %startTime%
echo End:      %endTime%
echo Elapsed:  %hh:~1%%time:~2,1%%mm:~1%%time:~2,1%%ss:~1%%time:~8,1%%cc:~1%

