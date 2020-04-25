#pragma once

#include "Shroom.h"
#include "Input.h"

class Window
{
//Base
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

	//Window Exception
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

//Proc msg handling
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK HandleMsgThunk(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT HandleMsg(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

//Input
public:
	Input input;

//Data & Transform
private:
	int  width;
	int  height;
	HWND handle;

	void UpdateRect() noexcept;

//Class
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