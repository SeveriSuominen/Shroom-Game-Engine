#include "ShroomWindow.h"
#include "resource.h"

//--------------------------------------------------
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
Window::Window(int w, int h, const char* name, SHROOM_WINDOW_TYPE type, HWND parentHandle) : width(w), height(h), type(type) 
{
	//TEMP def Style
	DWORD style;

	switch (type)
	{
		case SHROOM_WINDOW_TYPE::MAIN:
			style = WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CAPTION | WS_SYSMENU;
			break;
		case SHROOM_WINDOW_TYPE::SECONDARY:
			style = WS_THICKFRAME | WS_CAPTION;
			break;
	}

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
		parentHandle,
		nullptr,
		WindowClass::GetInstance(),
		this
	);
	
	if(hWnd == nullptr)
		throw SWND_EXC_LAST(); //Window error

	//Create graphic object
	#ifdef __SHROOM_ARCANE_3D__ 
	pGfx = std::make_unique<ShroomArcaneGraphics>( hWnd );
	#endif

	//Check if we have GFX instance
	if (pGfx == nullptr)
		SWND_NO_GFX_EXC();

	ShowWindow(hWnd, SW_SHOW);	

	ImGui_ImplWin32_Init( hWnd );
}

Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(handle);
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}

		TranslateMessage (&msg);
		DispatchMessage  (&msg);
	}
	return {};
}
//--------------------------------------------------
