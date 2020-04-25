#include "ShroomArcaneGraphics.h"
#include <sstream>
#include "dxerr.h"

ShroomArcaneGraphics::HRException::HRException(int line, const char * file, HRESULT hr) noexcept : hr(hr), Exception(line, file){}

const char * ShroomArcaneGraphics::HRException::what() const noexcept
{
	/*std::ostringstream oss;
	oss << GetType() << std::endl
		<< hr;

	whatBuffer = oss.str();
	return whatBuffer.c_str();*/

	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;

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

//Special case
const char * ShroomArcaneGraphics::DeviceRemovedException::GetType() const noexcept
{
	return "Shroom Arcane Graphics Exception [DEVICE REMOVED] (DXGI_ERROR_DEVICE_REMOVED)";
}
