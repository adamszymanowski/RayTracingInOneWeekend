# RayTracingInOneWeekend

My attempt at doing the 
[Ray Tracing in One Weekend](https://www.realtimerendering.com/raytracing/Ray%20Tracing%20in%20a%20Weekend.pdf)

More info at their [github repo](https://github.com/RayTracing/raytracing.github.io)


The idea is to do the book, but using Win32 Window as a renderer.

I want to utilize simple win32 code from Handmade Hero.
- [Handmade Hero Day 002](https://www.youtube.com/watch?v=4ROiWonnWGk)
- [Handmade Hero Day 003 - Allocating a Backbuffer](https://www.youtube.com/watch?v=GAi_nTx1zG8)

But I want to use just Visual Studio in the process.

NOTE: There, were problems... solutions below!
# Documentation

## Entry point errors
Avoiding dumb
- C28251
- LNK2019
- LNK1120
errors

Solution Explorer:
RayTracingInOneWeekend -> Properties -> (look for options and switches): Entry -> Entry Point -> (enter value):
WinMainCRTStartup

[/ENTRY (Entry-Point Symbol)](https://docs.microsoft.com/en-us/cpp/build/reference/entry-entry-point-symbol?redirectedfrom=MSDN&view=msvc-160)

## error C2440
cannot convert from 'const char [12]' to 'LPCWSTR'

RayTracingInOneWeekend -> Properties ->General -> Character Set -> (choose value):
Use Multi-Byte Character Set

## Other
[Window Class Styles](https://docs.microsoft.com/en-us/windows/win32/winmsg/window-class-styles)

### System defined messages (in WindowProc)
[WindowProc callback function](https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms633573(v=vs.85))
[About Messages and Message Queues](https://docs.microsoft.com/pl-pl/windows/win32/winmsg/about-messages-and-message-queues?redirectedfrom=MSDN)
[Window Notifications](https://docs.microsoft.com/pl-pl/windows/win32/winmsg/window-notifications)

### CreateWindow
[CreateWindowExA function (winuser.h)](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa)
[Window Styles](https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles)

### Top-down rendering
[BITMAPINFOHEADER structure](https://docs.microsoft.com/en-us/previous-versions/dd183376(v=vs.85))
