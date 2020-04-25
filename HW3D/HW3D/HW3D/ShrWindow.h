#pragma once

#include "Shroom.h"
#include "Input.h"
#include <memory>

//--------------------------------------------------
//ARCANE
//--------------------------------------------------
#include "ShroomArcane3D/ShroomArcane3D.h"
//--------------------------------------------------

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
//Exceptios
//--------------------------------------------------
	class Exception : public ShroomException 
	{
	public:
		Exception(int line, const char* file, HRESULT hr);
		const char* what()  const  noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		
	private:
		HRESULT hr;
	};

	class NoGFXException : public ShroomException
	{
	public:
		NoGFXException(int line, const char* file);
		const char* what()  const  noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;

	private:
		HRESULT hr;
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