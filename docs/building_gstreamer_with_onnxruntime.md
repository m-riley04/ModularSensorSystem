# GStreamer with ONNX using Meson on Windows

## General Notes

- Commands should be run within the *x64 Native Tools Command Prompt for VS 2022* (a.k.a. within the Visual Studio env).
- Commands can be ran as either user or admin, but I prefer admin just in case.

## Environment Variables

### General

- `DEV_ROOT`: the root dir for where you want the repositories and dependency folder to live
- `DEPS_ROOT`: the root dir for where you want the installed builds to live

### onnxruntime

- `ORT_SRC`: The root source directory for the ORT repo
- `ORT_BUILD`: The build directory ***with platform and config*** (i.e. build/Windows/RelWithDebugInfo)
- `ORT_INSTALL_PREFIX`: The directory where installation files will go for ORT

### GStreamer

- `ORT_ROOT`: Your INSTALLED ORT directory
- `GST_ROOT`: Your INSTALLED GStreamer directory (or `GSTREAMER_1_0_ROOT_MSVC_X86_64`)
  - `GSTREAMER_1_0_ROOT_MSVC_X86_64`: This is the path that GStreamer looks for in it's documentation. I usually set THIS to the installed path, and then I set GST_ROOT to this.
- `ORT_PKGCFG`: The pkg-config path for ORT. Usually, this will be `%ORT_ROOT%\lib\pkgconfig`
- `Path` (or `PATH`)
  - `%ORT_ROOT%\bin`  
  - `%GST_ROOT%\bin`

## Building onnxruntime

### Commands (cmd)

Partly taken from [gstonnx's documentation](https://gitlab.freedesktop.org/gstreamer/gstreamer/-/tree/1.26/subprojects/gst-plugins-bad/ext/onnx?ref_type=heads) (slightly outdated) and the official [ORT docs for  building on Windows for inferencing](https://onnxruntime.ai/docs/build/inferencing.html#windows).

The most notable changes are:

- Not using v1.16.3 (hash mismatches when building). Instead, using v1.23.0.

```bash

cd %DEV_ROOT%
git clone --recursive https://github.com/microsoft/onnxruntime.git
cd onnxruntime
git checkout -b v1.23.0 refs/tags/v1.23.0

.\build.bat --config RelWithDebInfo --build_shared_lib --parallel --compile_no_warning_as_error --skip_submodule_sync

cmake -S %ORT_SRC%\cmake -B %ORT_BUILD% -DCMAKE_INSTALL_PREFIX="%ORT_INSTALL_PREFIX%"
cmake --install %ORT_BUILD% --config RelWithDebInfo

```

## Building GStreamer

You may need to modify gstonnxprovider.cpp on Windows before running these.
<!-- TODO: add more information -->

```bash

cd %GST_ROOT%
rmdir /s /q builddir

set "INCLUDE=%ORT_ROOT%\include\onnxruntime;%INCLUDE%"

meson setup builddir ^
  --prefix %GST_ROOT% ^
  --buildtype=debugoptimized ^
  -Dpkg_config_path="%ORT_PKGCFG%" ^
  -Dintrospection=disabled ^
  -Dgpl=enabled ^
  -Dgst-plugins-bad:onnx=enabled ^
  -Dgst-plugins-bad:tensordecoders=enabled ^
  -Dgst-plugins-ugly:x264=enabled
  
meson compile -C builddir

meson install -C builddir

```

Or, if you've already ran setup and need to reconfigure:

```bash
meson configure builddir ^
  --prefix %GST_ROOT% ^
  --buildtype=debugoptimized ^
  -Dpkg_config_path="%ORT_PKGCFG%" ^
  -Dintrospection=disabled ^
  -Dgpl=enabled ^
  -Dgst-plugins-bad:onnx=enabled ^
  -Dgst-plugins-bad:tensordecoders=enabled ^
  -Dgst-plugins-ugly:x264=enabled
```
