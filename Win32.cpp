#include <windows.h>


#define global_variable static
#define internal_function static

global_variable int Scale = 2;
global_variable int BitmapWidth = 256*Scale;
global_variable int BitmapHeight = 128*Scale;


global_variable int WindowWidth;
global_variable int WindowHeight;

global_variable BITMAPINFO BitmapInfo;
global_variable void* BitmapMemory;

// Ray Tracing In One Weekend (setup)
global_variable int NumberOfSamples = 8; // for live rendering this has to be a low value, otherwise it's super slow...

#include "float.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include <random>

internal_function inline float // taken from the updated version: https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects
random_float()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	float rand = distribution(generator);
	return rand;
}

internal_function vec3
random_in_unit_sphere()
{
	vec3 p;
	do
	{
		p = 2.0 * vec3(random_float(), random_float(), random_float()) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

internal_function vec3
color(const ray& r, hitable *world)
{
	hit_record rec;
	if (world->hit(r, 0.0, FLT_MAX, rec))
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5 * color(ray(rec.p, target-rec.p), world);
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
	lambertian(const vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return true;
	}

	vec3 albedo;
};

// Ray Tracing In One Weekend (setup END)



internal_function void
Win32ResizeDIBSection()
{
	if (BitmapMemory)
	{
		VirtualFree(BitmapMemory, 0, MEM_RELEASE);
	}

	BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
	BitmapInfo.bmiHeader.biWidth = BitmapWidth;
	// this should be negative height https://docs.microsoft.com/en-us/previous-versions/dd183376(v=vs.85) to make
	// "The rows are written out from top to bottom" true, but whatever!
	BitmapInfo.bmiHeader.biHeight = BitmapHeight; 
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;
	
	int BytesPerPixel = 4;	
	int BitmapMemorySize = BitmapWidth * BitmapHeight * BytesPerPixel;
	int Pitch = BitmapWidth * BytesPerPixel;
	BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
	
	// Ray Tracing In One Weekend Rendering (setup)
	hitable* list[2];
	list[0] = new sphere(vec3(0, 0, -1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hitable* world = new hitable_list(list, 2);
	camera cam;
	// Ray Tracing In One Weekend Rendering (setup END)

	unsigned char *Row = (unsigned char *)BitmapMemory;
	for (int Y = 0; Y < BitmapHeight; Y++)
	{
		unsigned char *Pixel = (unsigned char *)Row;
		for (int X = 0; X < BitmapWidth; X++)
		{
			// Ray Tracing In One Weekend Rendering (actual rendering)
			vec3 ColorOutput(0, 0, 0);
			for (int S = 0; S < NumberOfSamples; S++)
			{
				float u = float(X + random_float()) / float(BitmapWidth);
				float v = float(Y + random_float()) / float(BitmapHeight);
				ray r = cam.get_ray(u, v);
				ColorOutput += color(r, world);
			}

			ColorOutput /= float(NumberOfSamples); // Comment this out for psychodelique fun!

			ColorOutput = vec3(sqrt(ColorOutput[0]), sqrt(ColorOutput[1]), sqrt(ColorOutput[2]));

			int ir = int(255.99 * ColorOutput[0]);
			int ig = int(255.99 * ColorOutput[1]);
			int ib = int(255.99 * ColorOutput[2]);

			*Pixel = ib;		// B
			++Pixel;

			*Pixel = ig;		// G
			++Pixel;

			*Pixel = ir;		// R
			++Pixel;
			// Ray Tracing In One Weekend Rendering (actual rendering END)

			*Pixel = 0;
			++Pixel;
		}

		Row += Pitch;
	}
}

internal_function void
Win32UpdateWindow(HDC DeviceContext, RECT *WindowRect, int X, int Y, int Width, int Height)
{
	int WindowWidth = WindowRect->right - WindowRect->left;
	int WindowHeight = WindowRect->bottom - WindowRect->top;

	StretchDIBits(
		DeviceContext,
		0, 0, WindowWidth, WindowHeight, // Destination
		0, 0, BitmapWidth, BitmapHeight, // Source
		BitmapMemory,
		&BitmapInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
}

LRESULT CALLBACK 
Win32MainWindowCallback(
	HWND   Window,
	UINT   Message,
	WPARAM WParam,
	LPARAM LParam)

{
	LRESULT Result = 0;
	switch (Message)
	{
		case WM_SIZE:
		{
			Win32ResizeDIBSection();
			OutputDebugStringA("WM_SIZE\n");
		} break;

		case WM_CLOSE:
		{
			OutputDebugStringA("WM_CLOSE\n");
			DestroyWindow(Window);
		} break;

		case WM_DESTROY:
		{
			OutputDebugStringA("WM_DESTROY\n");
			PostQuitMessage(0);
			Result = 0;
		} break;

		case WM_ACTIVATEAPP:
		{
			OutputDebugStringA("WM_ACTIVATEAPP\n");
		} break;

		case WM_PAINT:
		{
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			int X = Paint.rcPaint.left;
			int Y = Paint.rcPaint.top;
			int Width = Paint.rcPaint.right - Paint.rcPaint.left;
			int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;

			RECT ClientRect;
			GetClientRect(Window, &ClientRect);

			Win32UpdateWindow(DeviceContext, &ClientRect, X, Y, Width, Height);
			EndPaint(Window, &Paint);
		} break;

		default:
		{
			OutputDebugStringA("default\n");
			Result = DefWindowProc(Window, Message, WParam, LParam);
		}
	}

	return Result;
}

int WINAPI 
WinMain(
	HINSTANCE Instance,
	HINSTANCE PrevInstance,
	LPSTR     CommandLine,
	int       ShowCode)
{
	WNDCLASS WindowClass = {};
	
	WindowClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	WindowClass.lpfnWndProc = Win32MainWindowCallback;
	WindowClass.hInstance = Instance;
	//WindowClass.hIcon;
	WindowClass.lpszClassName = "RTIOW_Class";

	

	if (RegisterClass(&WindowClass))
	{
		HWND WindowHandle = CreateWindowExA(
			0,
			WindowClass.lpszClassName,
			"Ray Tracing In One Weekend",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,		// X
			CW_USEDEFAULT,		// Y
			CW_USEDEFAULT,	// width
			CW_USEDEFAULT,	// height
			0,
			0,
			Instance,
			0
		);

		if (WindowHandle)
		{
			// This need to be a part of code for some reason
			ShowWindow(WindowHandle, ShowCode);

			MSG Message;
			for(;;)
			{
				

				BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
				if (MessageResult > 0)
				{
					TranslateMessage(&Message);
					DispatchMessageA(&Message);
				}
				else
				{
					break;
				}

			}

		}
		else
		{
			// TODO: Logging
		}
	}
	else
	{
		// TODO: Logging
	}
}