#include "ShroomArcane3D.h"
#include <sstream>

ShroomArcaneException::ShroomArcaneException(int line, const char * file) noexcept
	: line(line), file(file) {}

const char * ShroomArcaneException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char * ShroomArcaneException::GetType() const noexcept
{
	return "Shroom Exception";
}

int ShroomArcaneException::GetLine() const noexcept
{
	return line;
}

const std::string & ShroomArcaneException::GetFile() const noexcept
{
	return file;
}

std::string ShroomArcaneException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}
