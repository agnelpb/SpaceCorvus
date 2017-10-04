#include "main.h"
#include "GameEngine.h"

void main()
{
	GameEngine::Instance().initialize();                              // Initializes all systems
	GameEngine::Instance().gameLoop();								  // Starts game
}
