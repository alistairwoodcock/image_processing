//cl instructions.cpp gdi32.lib user32.lib Kernel32.lib && instructions.exe
#ifndef _image_win32
#define _image_win32
#include <windows.h>
#include <iostream>
#include "logger.h"


typedef struct win32_offscreen_buffer
{
	BITMAPINFO Info;
	void *Memory;
	int Width;
	int Height;
	int Pitch;
	int BytesPerPixel;

} ImageBuffer;

struct win32_window_dimension
{
	int Width;
	int Height;
};

ImageBuffer stage;
bool GlobalRunning = true;
HWND Window;

ImageBuffer new_image_buffer(int width, int height)
{
	ImageBuffer Buffer = {0};

	if(Buffer.Memory)
	{
		VirtualFree(Buffer.Memory, 0, MEM_RELEASE);
	}

	Buffer.Width = width;
	Buffer.Height = height;
	Buffer.BytesPerPixel = 4;

	Buffer.Info.bmiHeader.biSize = sizeof(Buffer.Info.bmiHeader);
	Buffer.Info.bmiHeader.biWidth = width;
	Buffer.Info.bmiHeader.biHeight = -height;
	Buffer.Info.bmiHeader.biPlanes = 1;
	Buffer.Info.bmiHeader.biBitCount = 32;
	Buffer.Info.bmiHeader.biCompression = BI_RGB;

	int BitmapMemorySize = (Buffer.Width*Buffer.Height)*Buffer.BytesPerPixel;

	Buffer.Memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

	Buffer.Pitch = width*Buffer.BytesPerPixel;

	return Buffer;
}

void delete_image_buffer(ImageBuffer* Buffer)
{
	if(Buffer->Memory)
	{
		VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
	}	
}


win32_window_dimension Win32GetWindowDimension(HWND Window)
{
	win32_window_dimension dimension;

	RECT ClientRect;
	GetClientRect(Window, &ClientRect);

	dimension.Width = ClientRect.right - ClientRect.left;
	dimension.Height = ClientRect.bottom - ClientRect.top;

	return dimension;
}

void Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int width, int height)
{
	if(Buffer->Memory)
	{
		VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
	}

	Buffer->Width = width;
	Buffer->Height = height;
	Buffer->BytesPerPixel = 4;

	Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
	Buffer->Info.bmiHeader.biWidth = width;
	Buffer->Info.bmiHeader.biHeight = -height;
	Buffer->Info.bmiHeader.biPlanes = 1;
	Buffer->Info.bmiHeader.biBitCount = 32;
	Buffer->Info.bmiHeader.biCompression = BI_RGB;

	int BitmapMemorySize = (Buffer->Width*Buffer->Height)*Buffer->BytesPerPixel;

	Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

	Buffer->Pitch = width*Buffer->BytesPerPixel;
}

void Win32DisplayBufferInWindow(HDC DeviceContext, win32_offscreen_buffer *Buffer, int WindowWidth, int WindowHeight)
{
	StretchDIBits(DeviceContext,
		0, 0, WindowWidth, WindowHeight,
		0, 0, Buffer->Width, Buffer->Height,
		Buffer->Memory,
		&Buffer->Info,
		DIB_RGB_COLORS, SRCCOPY);

}

bool up = false;
bool down = false;

LRESULT CALLBACK MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
	LRESULT Result = 0;

	switch(Message)
	{
		case WM_SIZE:
		{
		}
		break;

		case WM_DESTROY:
		{
			GlobalRunning = false;
		}
		break;

		case WM_CLOSE:
		{
			GlobalRunning = false;
		}
		break;

		case WM_PAINT:
		{
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			win32_window_dimension dimension = Win32GetWindowDimension(Window);
			HDC DeviceContextMem = CreateCompatibleDC(DeviceContext);
			BitBlt(DeviceContext, 0, 0, stage.Width, stage.Height, DeviceContextMem, 0, 0, SRCCOPY);
			DeleteDC(DeviceContextMem);
			EndPaint(Window, &Paint);
		}break;

		default:
		{
			Result = DefWindowProc(Window, Message, WParam, LParam);
		}
	}

	return Result;
}

static int Width = 0;
static int Height = 0;
static bool draw_once = false;

int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if(!Width) Width = 600;
	if(!Height) Height = 600; 
	bool has_drawn = false;
	
	setup();


	WNDCLASS WindowClass = {};

	WindowClass.style = CS_HREDRAW|CS_VREDRAW;
	WindowClass.lpfnWndProc = MainWindowCallback;
	WindowClass.hInstance = Instance;
	WindowClass.lpszClassName = "Image";
	

	if(RegisterClass(&WindowClass))
	{
		Window = CreateWindowEx(0, WindowClass.lpszClassName, "Image", WS_OVERLAPPEDWINDOW|WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, 0, 0, Instance, 0 );


		BOOL consoleAllocated = AllocConsole();

		if(consoleAllocated)
		{
			HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			freopen("CONIN$", "r", stdin);
			freopen("CONOUT$", "w", stdout);
			set_console(&consoleHandle);
			write_log("Log Initialised");
		}

		SetForegroundWindow(Window);

		win32_window_dimension dimension = Win32GetWindowDimension(Window);

		Win32ResizeDIBSection(&stage, dimension.Width, dimension.Height);

		//calls instruction.setup

		
		while(GlobalRunning)
		{
			if(Window)
			{
				MSG Message;

				while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
				{
					if(Message.message == WM_QUIT)
					{
						GlobalRunning = false;
					}

					TranslateMessage(&Message);
					DispatchMessage(&Message);
				}

				if(draw_once && !has_drawn)
				{
					draw();
					has_drawn = true;
				}
				else if(!draw_once)
				{
					draw();
				}

				HDC DeviceContext = GetDC(Window);

				win32_window_dimension dimension = Win32GetWindowDimension(Window);

				Win32DisplayBufferInWindow(DeviceContext, &stage, dimension.Width, dimension.Height);

				ReleaseDC(Window, DeviceContext);
			}
		}

	}

	FreeConsole();
	close_log();
	return 0;
}

#endif