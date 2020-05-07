#pragma once

#include "Shroom.h"
#include <filesystem>
#include <string>

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
};