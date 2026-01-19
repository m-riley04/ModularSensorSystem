@echo off

REM Dispatch: if first arg is a label, jump to it.
if /i "%~1"==":GetElapsed" shift & goto GetElapsed
goto :eof

:GetElapsed
REM Args (after shift): %1=startTime  %2=endTime  %3=outVarNameset "t1=%~1"
setlocal enabledelayedexpansion

set "t1=%~1"
set "t2=%~2"
set "outVar=%~3"

for /f "tokens=1-4 delims=:.," %%a in ("%t1%") do (
  set /a "cs1=(((1%%a-100)*3600)+((1%%b-100)*60)+(1%%c-100))*100+(1%%d-100)"
)
for /f "tokens=1-4 delims=:.," %%a in ("%t2%") do (
  set /a "cs2=(((1%%a-100)*3600)+((1%%b-100)*60)+(1%%c-100))*100+(1%%d-100)"
)

set /a "elap=cs2-cs1"
if !elap! lss 0 set /a "elap+=24*3600*100"

set /a "cc=elap%%100, elap/=100, ss=elap%%60, elap/=60, mm=elap%%60, hh=elap/60"

set "hh=0!hh!" & set "mm=0!mm!" & set "ss=0!ss!" & set "cc=0!cc!"
set "result=!hh:~-2!:!mm:~-2!:!ss:~-2!.!cc:~-2!"

endlocal & set "%outVar%=%result%"
exit /b 0