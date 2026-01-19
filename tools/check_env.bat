@echo off
if /i "%VERBOSE%"=="1" (echo on) else (echo off)

echo Checking required build environment...

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

echo Build environment check passed.
