#include "ShroomException.h"
#include <sstream>

ShroomException::ShroomException(int line, const char * file) noexcept
	: line(line), file(file) {}

const char * ShroomException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char * ShroomException::GetType() const noexcept
{
	return "Shroom Exception";
}

int ShroomException::GetLine() const noexcept
{
	return line;
}

const std::string & ShroomException::GetFile() const noexcept
{
	return file;
}

std::string ShroomException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}
