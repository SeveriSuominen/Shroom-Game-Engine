#include "ShrWindow.h"
#include "resource.h"

//MACROS
#define SWND_EXC(hr)    Window::Exception(__LINE__, __FILE__, hr)
#define SWND_EXC_LAST() Window::Exception(__LINE__, __FILE__, GetLastError())

//WINDOW CLASS
//--------------------------------------------------
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wclass = { 0 };

	wclass.cbSize		 = sizeof(wclass);
	wclass.style		 = CS_OWNDC;
	wclass.lpfnWndProc	 = HandleMsgSetup;
	wclass.cbClsExtra	 = 0;
	wclass.cbWndExtra	 = 0;
	wclass.hInstance	 = GetInstance();
	wclass.hIcon		 = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 32, 32, 0));
	wclass.hCursor		 = nullptr;
	wclass.hbrBackground = nullptr;
	wclass.hIconSm	     = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 16, 16, 0));;
	wclass.lpszClassName = GetName();

	RegisterClassEx(&wclass);
}

Window::WindowClass::~WindowClass() 
{
	UnregisterClass(wndClassName, GetInstance());
}

const char * Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

const HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}
//--------------------------------------------------

//WINDOW INTERFACE
//--------------------------------------------------
Window::Window(int w, int h, const char* name) : width(w), height(h)
{
	//TEMP def Style
	const DWORD style = WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CAPTION | WS_SYSMENU;

	RECT rect;
	rect.left   = 0;
	rect.right  = w;
	rect.top    = 0;
	rect.bottom = h;

	if(FAILED(AdjustWindowRect(&rect, style, FALSE)))
		throw SWND_EXC_LAST();

	HWND hWnd = CreateWindowEx
	(
		0,
		WindowClass::GetName(),
		name,
		style,				     //Style
		CW_USEDEFAULT,			 //X 
		CW_USEDEFAULT,			 //Y
		rect.right - rect.left,  //Width
		rect.bottom - rect.top,  //Height,
		nullptr,
		nullptr,
		WindowClass::GetInstance(),
		this
	);
	
	if(hWnd == nullptr)
		throw SWND_EXC_LAST(); //Window error

	ShowWindow(hWnd, SW_SHOW);	
}

Window::~Window()
{
	DestroyWindow(handle);
}
//--------------------------------------------------
