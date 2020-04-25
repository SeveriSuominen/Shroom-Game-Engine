#include "ShroomTimer.h"

using namespace std::chrono;

ShroomTimer::ShroomTimer()
{
	last = steady_clock::now();
}

//Time since last mark
float ShroomTimer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> timeBetween = last - old;
	return timeBetween.count();
}

//Time since start timer
float ShroomTimer::Peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}
