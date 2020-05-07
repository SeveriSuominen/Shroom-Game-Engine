#pragma once
#include <chrono>

class ShroomTimer
{
public: 
	ShroomTimer();
	float Mark();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point last;
};