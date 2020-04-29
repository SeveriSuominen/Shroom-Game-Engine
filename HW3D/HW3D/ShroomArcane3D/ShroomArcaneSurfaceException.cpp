#include "Surface.h"
#include  <sstream>

//----------------------------------------------------------------------------------
// Surface exception
//----------------------------------------------------------------------------------
Surface::Exception::Exception(int line, const char* file, std::string note) noexcept
	:
	ShroomArcaneException(line, file),
	note(std::move(note))
{}

const char* Surface::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << ShroomArcaneException::what() << std::endl
		<< "[Note] " << GetNote();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Surface::Exception::GetType() const noexcept
{
	return "Shroom Arcane Surface Exception";
}

const std::string& Surface::Exception::GetNote() const noexcept
{
	return note;
}
//----------------------------------------------------------------------------------