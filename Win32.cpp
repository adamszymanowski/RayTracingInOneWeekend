#include <windows.h>
#include <stdint.h>

typedef uint8_t ui8;

#define internal static
#define global_variable static

global_variable BITMAPINFO BitmapInfo;
global_variable void* BitmapMemory;

global_variable int BitmapWidth;
global_variable int BitmapHeight;
global_variable int WindowWidth;
global_variable int WindowHeight;

internal void
Win32ResizeDIBSection(int Width, int Height)
{
	if (BitmapMemory)
	{
		VirtualFree(BitmapMemory, 0, MEM_RELEASE);
	}

	BitmapWidth = Width;
	BitmapHeight = Height;

	BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
	BitmapInfo.bmiHeader.biWidth = BitmapWidth;
	BitmapInfo.bmiHeader.biHeight = -BitmapHeight; // why negative height (https://docs.microsoft.com/en-us/previous-versions/dd183376(v=vs.85))
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;
	
	int BytesPerPixel = 4;	
	int BitmapMemorySize = Width * Height * BytesPerPixel;
	int Pitch = Width * BytesPerPixel;
	BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

	ui8 *Row = (ui8 *)BitmapMemory;
	for (int Y = 0; Y < BitmapHeight; Y++)
	{
		ui8 *Pixel = (ui8 *)Row;
		for (int X = 0; X < BitmapWidth; X++)
		{
			*Pixel = 0;		// B
			++Pixel;

			*Pixel = 0;	// G
			++Pixel;

			*Pixel = 255;		// R
			++Pixel;

			*Pixel = 0;
			++Pixel;
		}

		Row += Pitch;
	}
}

internal void
Win32UpdateWindow(HDC DeviceContext, RECT *WindowRect, int X, int Y, int Width, int Height)
{
	int WindowWidth = WindowRect->right - WindowRect->left;
	int WindowHeight = WindowRect->bottom - WindowRect->top;

	StretchDIBits(
		DeviceContext,
		0, 0, BitmapWidth, BitmapHeight, 
		0, 0, WindowWidth, WindowHeight, 
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
			RECT ClientRect;
			GetClientRect(Window, &ClientRect);
			int Width = ClientRect.right - ClientRect.left;
			int Height = ClientRect.bottom - ClientRect.top;
			Win32ResizeDIBSection(Width, Height);
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

int WINAPI WinMain(
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
			100,		// X
			100,		// Y
			200,	// width
			100,	// height
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