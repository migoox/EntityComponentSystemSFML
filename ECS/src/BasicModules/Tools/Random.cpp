#include "Random.h"

Basic::Random Basic::Random::s_Instance;

Basic::Random::Random()
{
	srand(time(NULL));
}

float Basic::Random::IFloat()
{
	return (rand() % 100) / 100.0f;
}

float Basic::Random::Float()
{
	return s_Instance.IFloat();
}