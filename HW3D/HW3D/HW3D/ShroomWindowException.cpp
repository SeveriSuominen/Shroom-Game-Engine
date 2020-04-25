#include "ShrWindow.h"
#include "resource.h"

#include <string>
#include <sstream>
#include <iostream>


//Window Exception
//--------------------------------------------------
Window::Exception::Exception(int line, const char * file, HRESULT hr) : ShroomException(line, file), hr(hr) {}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error] " << GetErrorCode() << std::endl
		<< "[Desc] " << GetErrorString() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "Shroom Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hr, //Errorcode
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //Locale ID
		reinterpret_cast<LPSTR>(&pMsgBuf), //Needs pointer of pointer local &pMsgBuf
		0,
		nullptr
	);

	if (nMsgLen == 0)
		return "Unidentified";

	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf); //Free win32 allocated memory for msg buffer after copy 

	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}
//--------------------------------------------------
//No GFX Exception
//--------------------------------------------------
Window::NoGFXException::NoGFXException(int line, const char * file) : ShroomException(line, file) {}

const char* Window::NoGFXException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error] " << GetErrorCode() << std::endl
		<< "[Desc] " << GetErrorString() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::NoGFXException::GetType() const noexcept
{
	return "Shroom Window No GFX Exception";
}

std::string Window::NoGFXException::TranslateErrorCode(HRESULT hr) noexcept
{
	return "Window GFX is nullptr";
}

HRESULT Window::NoGFXException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::NoGFXException::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}
//--------------------------------------------------
