#include "ShroomArcaneGraphics.h"
#include <sstream>
#include "dxerr.h"

//------------------------------------------------------------------------------------------
//HRException
//------------------------------------------------------------------------------------------
ShroomArcaneGraphics::HRException::HRException(int line, const char * file, HRESULT hr) noexcept : hr(hr), Exception(line, file) {}
ShroomArcaneGraphics::HRException::HRException(int line, const char * file, HRESULT hr, std::vector<std::string> trace) noexcept : hr(hr),Exception(line, file)
{
	//Create info message from dx debug trace
	for (const auto& ref : trace) 
	{
		info += ref;
		info.push_back('\n');
	}

	if (!info.empty())
		info.pop_back();
}

const char * ShroomArcaneGraphics::HRException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;

	if (!info.empty())
	{
		oss << "[Error Info]\n" << GetErrorInfo() << std::endl;
	}

	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char * ShroomArcaneGraphics::HRException::GetType() const noexcept
{
	return "Shroom Arcane Graphics Exception";
}

HRESULT ShroomArcaneGraphics::HRException::GetErrorCode() const noexcept
{
	return hr;
}

std::string ShroomArcaneGraphics::HRException::GetErrorString() const noexcept
{
	return DXGetErrorString(hr);
}

std::string ShroomArcaneGraphics::HRException::GetErrorDescription() const noexcept
{
	char   buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	return buf;
}

std::string ShroomArcaneGraphics::HRException::GetErrorInfo() const noexcept
{
	return info;
}
//------------------------------------------------------------------------------------------
//Info exception
//------------------------------------------------------------------------------------------
ShroomArcaneGraphics::InfoException::InfoException(int line, const char * file, std::vector<std::string> trace) noexcept : Exception(line, file)
{
	//Create info message from dx debug trace
	for (const auto& ref : trace)
	{
		info += ref;
		info.push_back('\n');
	}

	if (!info.empty())
		info.pop_back();
}

const char * ShroomArcaneGraphics::InfoException::what() const noexcept
{
	std::ostringstream oss;

	if (!info.empty())
	{
		oss << GetType() << std::endl << std::endl 
		<< "[Error Info]\n" << GetErrorInfo() << std::endl;
	}

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char * ShroomArcaneGraphics::InfoException::GetType() const noexcept
{
	return "Shroom Arcane Graphics Info Exception";
}

std::string ShroomArcaneGraphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}

//------------------------------------------------------------------------------------------
//Device removed exception
//------------------------------------------------------------------------------------------
//Special case
const char * ShroomArcaneGraphics::DeviceRemovedException::GetType() const noexcept
{
	return "Shroom Arcane Graphics Exception [DEVICE REMOVED] (DXGI_ERROR_DEVICE_REMOVED)";
}
//------------------------------------------------------------------------------------------