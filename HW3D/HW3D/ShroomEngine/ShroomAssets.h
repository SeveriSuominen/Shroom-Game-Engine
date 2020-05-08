#pragma once

#include "Shroom.h"
#include <filesystem>
#include <string>
#include <sstream>
#include <codecvt>

#include <boost\filesystem.hpp>

class ShroomAssets
{

private:
	static inline bool file_exists(const std::string& name) {
		struct stat buffer;
		return (stat(name.c_str(), &buffer) == 0);
	}

public:

	static boost::filesystem::path Path()
	{
		boost::filesystem::path path;

		if (file_exists(boost::filesystem::current_path().append("Assets").string()))
		{
			path = boost::filesystem::path(boost::filesystem::current_path().append("Assets"));
		}
		else
		{

			//boost::filesystem::current_path().

#ifdef  SRELEASE
		path = boost::filesystem::path(boost::filesystem::current_path().append("..\\x64\\Release\\Assets"));
#else
		path = boost::filesystem::path(boost::filesystem::current_path().append("..\\x64\\Debug\\Assets"));
#endif
		}
		return path;
	}

	static bool File_exists(const std::string& name) {
		struct stat buffer;
		return (stat(name.c_str(), &buffer) == 0);
	}

	static std::wstring wstr(const std::string str)
	{//
		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;

		std::wstring str2(str.length(), L' '); // Make room for characters

		 // Copy string to wstring.
		std::copy(str.begin(), str.end(), str2.begin());

		return str2;
	}

	template<class Shd>
	static std::unique_ptr<Shd> Get_shader(ShroomArcaneGraphics& gfx, const std::string& name)
	{
		std::stringstream ss;
		ss << "Shaders/" << name;

		std::stringstream ss2;
		ss2 << "../ShroomArcane3D/" << name;

		std::stringstream cur;
		cur << std::filesystem::current_path();

		//std::wstring wss;
		//wss = std::wstring(ss.str().c_str().begin(), ss.str().c_str().end());
		if (file_exists(ss.str().c_str()))
		{
			return std::make_unique<Shd>(gfx, wstr(ss.str()));
		}
		else
		{
			return std::make_unique<Shd>(gfx, wstr(ss2.str()));
		}
	}
	// __SNIPPET
};