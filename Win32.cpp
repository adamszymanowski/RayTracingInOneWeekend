#include <windows.h>


int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd)
{
	MessageBoxA(0, "This is Win32 Window.", "Win32 Window", MB_OK | MB_ICONINFORMATION);
	/*
	WNDCLASS WindowClass = {};
	
	WindowClass.style = ;
	WindowClass.lpfnWndProc = ;
	WindowClass.cbClsExtra = ;
	WindowClass.cbWndExtra = ;
	WindowClass.cbWndExtra = ;
	WindowClass.hInstance = ;
	WindowClass.hIcon = ;
	WindowClass.hCursor = ;
	WindowClass.hbrBackground = ;
	WindowClass.lpszMenuName = ;
	WindowClass.lpszClassName = ;
	*/
}