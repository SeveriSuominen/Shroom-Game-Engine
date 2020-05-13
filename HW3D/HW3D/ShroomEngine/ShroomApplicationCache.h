#pragma once

#include <filesystem>

struct ShroomApplicationCache
{
public: 
	void SetCurrentSave(std::filesystem::path current_save_file) { this->current_save_file = current_save_file; }
	std::filesystem::path& GetCurrentSave() { return this->current_save_file; }

	void ResetCache()
	{
		this->current_save_file = std::filesystem::path();
	}
private:
	mutable std::filesystem::path current_save_file;
};