#pragma once
#include <string>
#include <filesystem>
class ShroomWin32Tools
{
public:
	static std::filesystem::path OpenFileDialog();
	static std::filesystem::path SaveAsDialog();
};