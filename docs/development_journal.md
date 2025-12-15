# Development Journal

This is a journal for storing things I've learned over the course of this project that can hopefully help myself (and potentially others). My pain, your resolve!

## Common Issues

- "Heap Corruption Detected" on Windows when closing the application
    - 9 times out of 10, a clean and rebuild of the solution will fix this. Probably some Qt Meta thing.

## Linker Errors

In each project, check each of these places in Properties:

- VC++ -> General -> Include Directories
- VC++ -> General -> Library Directories
- C++ -> General -> Additional Include Directories
- Linker -> Input -> Additional Dependencies
- Qt Meta-Object Compiler -> Additional Include Directories

### GStreamer

Ensure the following first and foremost:

1. Both the developer build and release build of GStreamer is installed via MSI on your system
2. Ensure that your environment variables are correctly set up, specifically:
    - MAKE SURE THAT GST/GLIB DLLS AREN'T BEING SHADOWED IN PATH!!! This is specifically an issue with GraphViz, since GStreamer uses it for debugging.

