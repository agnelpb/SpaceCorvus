#include "GameEngine.h"
#include "Object.h"
#include "Parser.h"
#include "Player.h"
#include "FileSystem.h"
#include "gameLevel.h"
#include <iostream>


GameEngine* GameEngine::instance = NULL;


void GameEngine::initialize()
{
	std::cout << "Initializing Game..." << std::endl;
	FileSystem::Instance();										// initializing File System
	gameLevel::Instance();										// initializing gameLevel
	Parser::Instance();											// Initializing Parser


	std::cout << "Loading Game Level..." << std::endl;
	FileSystem::Instance().loadGameLevel("../Resources/gameLevel.xml");

	Player::Instance();	// initializing player
	
	std::cout << " \n \n :::::::::::  SPACE CORVUS :::::::: \n \n " << std::endl;
	std::cout << " We have received a distress signal from the SASS NewDawn. \n A robot milita has attacked the space station and \n is trying to take control of the station. \n There have been multiple human casualties.   " << std::endl;
	std::cout << " Your mission is to infiltrate the ship and neutralize the situation. \n Read the briefing in the manual before starting the mission and use the map to help in navigation. Type Help for commands. \n \n " << std::endl;
	

	std::cout << " Your spacecraft lands on the landing bay of the ship...\n \n " << std::endl;

}

void GameEngine::gameLoop()
{
	
	Player::Instance().describe();																		// describe the start room

	while (true)  // infinite game Loop
	{
		std::cout << "\n \t \t Player Health : " << Player::Instance().health << " \t Weapon : "<< Player::Instance().currentWeapon->getName() << std::endl;	
		Player::Instance().hostileCheck();							// Check if any hostile enemies are in room. If so, directly enter combat.
		if(!Player::Instance().inCombat)							// If not attacking, Get player commands.
			Parser::Instance().getCommand();
	}
}


