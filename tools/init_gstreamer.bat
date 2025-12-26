@echo off
if /i "%VERBOSE%"=="1" (echo on) else (echo off)

:gstreamer
echo === GStreamer ===

:gstreamer_fetch
echo Fetching source code...
IF NOT EXIST "%GST_SRC%" (
	echo Cloning...
	git clone --recursive https://gitlab.freedesktop.org/gstreamer/gstreamer.git "%GST_SRC%" || exit /b 1
)
pushd "%GST_SRC%" || exit /b 1
set "gst_version=1.26.9"
set "gst_version_short=1.26"
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
  -Dcpp_std=c++17 ^
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
echo Build succeeded.

:gstreamer_install
echo Installing with Meson...
meson install -C "%meson_build_dir%" || exit /b 1
echo GStreamer installed to "%GST_ROOT%".

echo GStreamer initialization complete.

popd
