#pragma once

#include "Shroom.h"
#include "Input.h"
#include <memory>

//--------------------------------------------------
//ARCANE
//--------------------------------------------------
#include "ShroomArcane3D/ShroomArcane3D.h"
//--------------------------------------------------
#include "ShroomThrowMacros.h"

class Window
{
//--------------------------------------------------
//Base
//--------------------------------------------------
public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void SetTitle(const char* text)
	{
		SetWindowText(handle, text);
	};
	
	static std::optional<int> ProcessMessages();
//--------------------------------------------------
//Exceptions
//--------------------------------------------------
	class Exception : public ShroomException 
	{
		using ShroomException::ShroomException;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;	

	};
	class HRException : public Exception
	{
	public:
		HRException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGFXException : public Exception
	{
 		
	public:
		using Exception::Exception;
		virtual const char* GetType() const noexcept override;
	};
//--------------------------------------------------
//Graphics
//--------------------------------------------------
#ifdef __SHROOM_ARCANE_3D__
public:
	ShroomArcaneGraphics& Gfx() {return  *pGfx;}
private:
	std::unique_ptr<ShroomArcaneGraphics> pGfx;
#endif
//--------------------------------------------------
//Proc msg handling
//--------------------------------------------------
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK HandleMsgThunk(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT HandleMsg(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
//--------------------------------------------------
//Input
//--------------------------------------------------
public:
	Input input;
//--------------------------------------------------
//Data & Transform
//--------------------------------------------------
private:
	int  width;
	int  height;
	HWND handle;
//--------------------------------------------------
//Class
//--------------------------------------------------
private:
	//SINGLETON
	class WindowClass 
	{
	public: 
		static const char*	   GetName() noexcept;
		static const HINSTANCE GetInstance() noexcept;

	private:
		WindowClass() noexcept;
	   ~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName  = "HW3D";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
};
//--------------------------------------------------