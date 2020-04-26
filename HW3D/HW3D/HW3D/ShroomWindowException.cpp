#include "ShrWindow.h"
#include "resource.h"

#include <string>
#include <sstream>
#include <iostream>


//Window Exception
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

Window::HRException::HRException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file),
	hr(hr)
{}

const char* Window::HRException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::HRException::GetType() const noexcept
{
	return "Shroom Window Exception";
}

HRESULT Window::HRException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::HRException::GetErrorDescription() const noexcept
{
	return Exception::TranslateErrorCode(hr);
}


const char* Window::NoGFXException::GetType() const noexcept
{
	return "Shroom NoGFX Exception";
}