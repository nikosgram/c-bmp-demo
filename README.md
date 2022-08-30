# c-bmp-demo

A simple Bitmap (BMP) functionality demo written in C without dependencies.

## Example

![Example screenshot 0](screenshot-0.jpg)

## Research Sources

This demo is a 2×2 pixel, 24-bit bitmap (Windows DIB header BITMAPINFOHEADER) with pixel format RGB24 implementation.
The following sources been used for research purposes:

- https://en.wikipedia.org/wiki/BMP_file_format
- https://docs.microsoft.com/en-us/windows/win32/gdi/about-bitmaps
- https://docs.microsoft.com/en-us/previous-versions//dd183376(v=vs.85)

### Why CMake?

The only reason I choose to go down the CMake approach for this demo is because the IDE I used (CLion) has a native
support for CMake and allowed me to setup the project without me putting the effort of creating a build
script. [Kenneth](https://github.com/kstandbridge) at his [kengine](https://github.com/kstandbridge/kengine) repo has a
perfect representation on how a build script should be used.

https://github.com/kstandbridge/kengine

## Dependencies and Libraries

This project is using no libraries or dependencies.
