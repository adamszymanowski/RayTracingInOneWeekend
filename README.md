# RayTracingInOneWeekend

My attempt at doing the 
[Ray Tracing in One Weekend](https://www.realtimerendering.com/raytracing/Ray%20Tracing%20in%20a%20Weekend.pdf)

More info at their [github repo](https://github.com/RayTracing/raytracing.github.io)


The idea is to do the book, but using Win32 Window as a renderer.

I want to utilize simple win32 code from Handmade Hero.
[Handmade Hero Day 002](https://www.youtube.com/watch?v=4ROiWonnWGk&t=5s)

But I want to use just Visual Studio in the process.
NOTE: There, were problems... solutions below!

# Help on Visual Studio setup

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

## next
