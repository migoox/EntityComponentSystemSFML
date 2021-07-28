#pragma once
#include <chrono>
#include <iostream>

struct Timer
{
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;
	std::chrono::duration<float> duration;

	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	float CurrentTimeDuration()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		return duration.count() * 1000.0f;
	}
	~Timer()
	{
		float ms = CurrentTimeDuration();
		std::cout << "Timer took " << ms << " ms.\n";
	}
};