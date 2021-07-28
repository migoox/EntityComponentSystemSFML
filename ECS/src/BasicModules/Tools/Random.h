#pragma once
#include <time.h>
#include <cstdlib>

namespace Basic {
	class Random
	{
	private:
		static Random s_Instance;

	private:
		Random();
		
		float IFloat();

	public:
		static float Float();
	};
}