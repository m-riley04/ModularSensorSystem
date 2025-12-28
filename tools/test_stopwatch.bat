@echo off

set "startTime=%time: =0%"

timeout /t 5

set "endTime=%time: =0%"
call "%~dp0time_utils.bat" :GetElapsed "%startTime%" "%endTime%" elapsed

REM print details
echo --- Run details ---
echo Start:    %startTime%
echo End:      %endTime%
echo Elapsed:  %elapsed%