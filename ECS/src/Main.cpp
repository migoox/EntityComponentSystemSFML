#include "Game.h"
#include "Scenes/GameScene.h"
int main()
{
	// set start scene
	Framework::Game::SetScene(new GameScene());

	// run game
	Framework::Game::Run();
	
	return 0;
}