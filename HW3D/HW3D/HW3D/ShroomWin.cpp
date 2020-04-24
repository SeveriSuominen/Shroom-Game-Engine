#include "ShrWindow.h"

#include <string>
#include <sstream>

//EXAMPLE PROCS
LRESULT CALLBACK WndProc
(
	HWND   handle,
	UINT   msg,
	WPARAM wparam,
	LPARAM lparam
)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(5000);
		break;
	case WM_KEYDOWN:
		break;
	case WM_KEYUP:
		//EXAMPLE
		if (wparam == 'F')
		{

		}
		break;
	case WM_CHAR:
	{
		//EXAMPLE
		static std::string title;
		title.push_back((char)wparam);
		SetWindowText(handle, title.c_str());
	}
	break;
	//MOUSE
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		std::ostringstream oss;
		oss << "(" << pt.x << ", " << pt.y << ")";
		SetWindowText(handle, oss.str().c_str());
	}
	break;
	}
	return DefWindowProc(handle, msg, wparam, lparam);
}

int CALLBACK  WinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int		  nCmdShow
) 
{
	try {

		Window wnd1(600, 400, "Shroom");

		BOOL result;
		MSG  msg;

		/*Nullptr handle == all thread windows, remeber that with WProcs*/
		while ((result = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage (&msg);

			//TEST;
			if (wnd1.input.keyboard.KeyIsPressed(VK_SPACE))
				MessageBox(nullptr, "Yolo", "yellow", MB_OK);
		}
		return result != -1 ? msg.wParam : -1;
	}
	catch (const ShroomException& e)
	{
		ShroomExceptionHandler::Handle(nullptr, e.what(), e.GetType());
	}
	catch (const std::exception&  e)
	{
		ShroomExceptionHandler::Handle(nullptr, e.what(), "Standard Exception");
	}
	catch (...)
	{
		ShroomExceptionHandler::Handle(nullptr, "No details", "Unknown Exception");
	}
	return -1;
}
