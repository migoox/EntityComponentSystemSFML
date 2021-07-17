#include "Game.h"
#include "Scenes/GameScene.h"
#include <iostream>

struct AllocationMetrics
{
	uint32_t TotalAllocated = 0;
	uint32_t TotalFreed = 0;

	uint32_t CurrentUsage() { return TotalAllocated - TotalFreed; }
};

static AllocationMetrics s_AllocationMetrics;

static void PrintMemoryUsage() { std::cout << "Total memory usage: " << s_AllocationMetrics.CurrentUsage() << "bytes\n"; }

void* operator new(size_t size)
{
	s_AllocationMetrics.TotalAllocated += size;

	PrintMemoryUsage();

	return malloc(size);
}

void operator delete(void* memory, size_t size)
{
	s_AllocationMetrics.TotalFreed += size;

	PrintMemoryUsage();

	free(memory);
}
int main()
{
	// set start scene
	Framework::Game::SetScene(new GameScene());

	// run game
	Framework::Game::Run();
	
	return 0;
}