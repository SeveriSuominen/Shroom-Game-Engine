#include "ShroomWindow.h"

#include <string>
#include <sstream>
#include <iostream>

//Handle msgs proc
LRESULT Window::HandleMsg(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
//**************************************************
//IMGUI
//**************************************************
	//INGUI MIGHT, BUT NORMALLY USES ANOTHER WAYS TO SIGNAL

	const auto imguiIO = ImGui::GetIO();

	if (ImGui_ImplWin32_WndProcHandler(handle, msg, wparam, lparam))
	{
		return true;
	}
//**************************************************
	switch (msg)
	{
//--------------------------------------------------
//PROGRAM
//--------------------------------------------------
	case WM_CLOSE:
		if (type == Window::SHROOM_WINDOW_TYPE::MAIN)
		{
			PostQuitMessage(5000);
			return 5000;
		}
		break;
//--------------------------------------------------
//INPUT
//--------------------------------------------------
	case WM_KILLFOCUS:  //Clear on loss of focus to prevent keystates staying true for non focused window
		input.keyboard.ClearState();
		break;
//--------------------------------------------------
//KEYBOARD
//--------------------------------------------------
	case WM_SYSKEYUP:   // <--- Enable cathcing system key events (Alt, F10) for example
	case WM_KEYUP:
		//BREAK IF IMGUI WANTS TO CAPTURE KEYBOARD
		if (imguiIO.WantCaptureKeyboard)
		{
			break;
		}
		input.keyboard.OnKeyPressed(static_cast<unsigned char>(wparam));
		break;
	case WM_SYSKEYDOWN: // <--- System key catch
	case WM_KEYDOWN:
		//BREAK IF IMGUI WANTS TO CAPTURE KEYBOARD
		if (imguiIO.WantCaptureKeyboard)
		{
			break;
		}
		if (!(lparam & 0x40000000 /*30 index bit != 1*/) || input.keyboard.AutorepeatIsEnabled())
			input.keyboard.OnKeyPressed(static_cast<unsigned char>(wparam));
		break;
	case WM_CHAR:
		//BREAK IF IMGUI WANTS TO CAPTURE KEYBOARD
		if (imguiIO.WantCaptureKeyboard)
		{
			break;
		}
		input.keyboard.OnChar(static_cast<unsigned char>(wparam));
		break;
//--------------------------------------------------
//MOUSE
//--------------------------------------------------
	case WM_MOUSEMOVE:
	{ 
		//BREAK IF IMGUI WANTS TO CAPTURE MOUSE
		if (imguiIO.WantCaptureMouse)
		{
			break;
		}

		//UPDATE WINDOW RECT ON MOUSE MOVE 
		//------------------------------------------
		RECT w_cur_rect; GetWindowRect(handle, &w_cur_rect);

	    width  = w_cur_rect.right  - w_cur_rect.left;
		height = w_cur_rect.bottom - w_cur_rect.top;
		//------------------------------------------

		const POINTS pt = MAKEPOINTS(lparam);
		short offset = 40; // WINDOW BORDER OFFSET TO ALLOW RESIZE WORK WITH OUR CAPTURE LOGIC 

		//EVENT AND SET CAPTURES
		//------------------------------------------
		if (pt.x >= offset && pt.x < width - offset && pt.y >= 0 && pt.y < height - offset)
		{
			input.mouse.OnMouseMove(pt.x, pt.y);

			if (!input.mouse.isInWindow) 
			{
				SetCapture(handle);
				input.mouse.OnMouseEnter();
			}
		}
		else 
		{
			// We want to still register mouse movement even if not in window when dragging 
			// if not, then release capture until back to window
			if (wparam & (MK_LBUTTON | MK_RBUTTON)) 
			{
				input.mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				input.mouse.OnMouseLeave();
			}	
		}	
		//------------------------------------------
	}
	break;
	case WM_LBUTTONUP:
		//BREAK IF IMGUI WANTS TO CAPTURE MOUSE
		if (imguiIO.WantCaptureMouse)
		{
			break;
		}
	{ const POINTS pt = MAKEPOINTS(lparam); input.mouse.OnLeftReleased(pt.x, pt.y);  }
	break;
	case WM_LBUTTONDOWN:
		//BREAK IF IMGUI WANTS TO CAPTURE MOUSE
		if (imguiIO.WantCaptureMouse)
		{
			break;
		}
	{ const POINTS pt = MAKEPOINTS(lparam); input.mouse.OnLeftPressed(pt.x, pt.y);   }
	break;
	case WM_RBUTTONUP:
		//BREAK IF IMGUI WANTS TO CAPTURE MOUSE
		if (imguiIO.WantCaptureMouse)
		{
			break;
		}
	{ const POINTS pt = MAKEPOINTS(lparam); input.mouse.OnRightReleased(pt.x, pt.y); }
	break;
	case WM_RBUTTONDOWN:
		//BREAK IF IMGUI WANTS TO CAPTURE MOUSE
		if (imguiIO.WantCaptureMouse)
		{
			break;
		}
	{ const POINTS pt = MAKEPOINTS(lparam); input.mouse.OnRightPressed(pt.x, pt.y);  }
	break;
	case WM_MOUSEWHEEL:
	{
		//BREAK IF IMGUI WANTS TO CAPTURE MOUSE
		if (imguiIO.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lparam);
		const short  delta = GET_WHEEL_DELTA_WPARAM(wparam);
		
		//Check delta value with WHEEL_DELTA macro and dispatch WheelUp or WhellDown event.
		input.mouse.OnWheelDelta(pt.x, pt.y, delta);
	}
	break;
//--------------------------------------------------
	}
	//Windows default messages (+1000)
	return DefWindowProc(handle, msg, wparam, lparam);
}

//Setup create proc 
LRESULT Window::HandleMsgSetup(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_NCCREATE /*Window created*/)
	{
		//lparam TO Window*
		const CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lparam);
		Window* const pWindow = static_cast<Window*>(pCreate->lpCreateParams);
		
		//Save handle info to window //LINKAGE
		pWindow->handle = handle;
		
		//CHANGE HANDLE  (link)
		SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		//CHANGE WINPROC (use thunk handler)
		SetWindowLongPtr(handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		//HANDLE
		return pWindow->HandleMsg(handle, msg, wparam, lparam);
	}
	//Fallback def
	return DefWindowProc(handle, msg, wparam, lparam);
}

//Linkage proc (AFTER SETUP)
LRESULT Window::HandleMsgThunk(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Window* const pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA));
	return pWindow->HandleMsg(handle, msg, wparam, lparam);
}

