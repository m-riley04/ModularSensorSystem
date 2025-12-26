@echo off
if /i "%VERBOSE%"=="1" (echo on) else (echo off)
ver >nul REM resets errorlevel

REM start stopwatch
set "startTime=%time: =0%"
timeout /t 5 /nobreak >nul

echo Cleaning dependencies...

REM initialize environment
call .\tools\init_env.bat || exit /b 1

REM clean ONNX Runtime
echo Removing ONNX Runtime installation...
rmdir /s /q "%DEPS_ROOT%\onnxruntime"
if errorlevel 1 (
    echo WARNING: Failed to remove "%DEPS_ROOT%\onnxruntime"
) else (
    echo Removed "%DEPS_ROOT%\onnxruntime"
)

REM clean GStreamer
echo Removing GStreamer installation...
rmdir /s /q "%DEPS_ROOT%\gstreamer"
if errorlevel 1 (
    echo WARNING: Failed to remove "%DEPS_ROOT%\gstreamer"
) else (
    echo Removed "%DEPS_ROOT%\gstreamer"
)

REM clean source directories
echo Removing ONNX Runtime source...
rmdir /s /q "%SRC_ROOT%\onnxruntime"
if errorlevel 1 (
    echo WARNING: Failed to remove "%SRC_ROOT%\onnxruntime"
) else (
    echo Removed "%SRC_ROOT%\onnxruntime"
)

echo Removing GStreamer source...
rmdir /s /q "%SRC_ROOT%\gstreamer"
if errorlevel 1 (
    echo WARNING: Failed to remove "%SRC_ROOT%\gstreamer"
) else (
    echo Removed "%SRC_ROOT%\gstreamer"
)

REM TODO: clean up env variables?

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