@echo off
if /i "%VERBOSE%"=="1" (echo on) else (echo off)

echo Checking required build tools...

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

echo Build tools check passed.
