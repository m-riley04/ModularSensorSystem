# GStreamer with ONNX using Meson on Windows

## Building onnxruntime

### Environment Variables

- $ORT_SRC: The root source directory for the ORT repo
- $ORT_BUILD: The build directory ***with platform and config*** (i.e. build/Windows/RelWithDebugInfo)
- $ORT_INSTALL_PREFIX: The directory where installation files will go for ORT

### Commands (cmd)

Partly taken from [gstonnx's documentation](https://gitlab.freedesktop.org/gstreamer/gstreamer/-/tree/1.26/subprojects/gst-plugins-bad/ext/onnx?ref_type=heads) (slightly outdated) and the official [ORT docs for  building on Windows for inferencing](https://onnxruntime.ai/docs/build/inferencing.html#windows).

The most notable changes are:

- Not using v1.16.3 (hash mismatches when building)

```cmd

git clone --recursive https://github.com/microsoft/onnxruntime.git
cd onnxruntime
git checkout -b v1.23.0 refs/tags/v1.23.0

.\build.bat --config RelWithDebInfo --build_shared_lib --parallel --compile_no_warning_as_error --skip_submodule_sync

cmake -S $ORT_SRC\cmake -B $ORT_BUILD -DCMAKE_INSTALL_PREFIX="$ORT_INSTALL_PREFIX"
cmake --install $ORT_BUILD --config RelWithDebInfo

```

## Building GStreamer

need to modify gstonnxprovider.cpp on windows before running these

### Environment Variables

- ORT_ROOT: Your INSTALLED ORT directory
- GST_ROOT: Your INSTALLED GStreamer directory (or GSTREAMER_1_0_ROOT_MSVC_X86_64)
  - GSTREAMER_1_0_ROOT_MSVC_X86_64: This is the path that GStreamer looks for in it's documentation. I usually set THIS to the installed path, and then I set GST_ROOT to this.
- ORT_PKGCFG: The pkg-config path for ORT. Usually, this will be `%ORT_ROOT%\lib\pkgconfig`

- PATH
  - %ORT_ROOT%\bin  
  - %GST_ROOT%\bin

### Commands (cmd)

```cmd

cd C:\dev\gstreamer

rmdir /s /q builddir

set "INCLUDE=%ORT_ROOT%\include\onnxruntime;%INCLUDE%"

meson setup builddir ^
  --prefix %GST_ROOT% ^
  -Dintrospection=disabled ^
  -Dgst-plugins-bad:onnx=enabled ^
  --buildtype=debugoptimized ^
  -Dpkg_config_path="%ORT_PKGCFG%"

meson compile -C builddir

meson install -C builddir

```
