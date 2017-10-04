#include "Room.h"
#include "gameLevel.h"
#include <iostream>

Room::Room()
{
	visited = false;
	locked = false;
}


Room::~Room()
{
}

// Search in each map and find out the type of object
std::string Room::findObjectType(std::string object)
{
	
	std::map <std::string, int>::iterator itr = pickables.begin();
	while (itr != pickables.end())
	{
		if (object == itr->first)
		{
			return "pickable";
		}
		itr++;
	}

	std::multimap <std::string, Enemy*>::iterator eItr = enemies.begin();
	while (eItr != enemies.end())
	{
		if (object == eItr->first)
		{
			return "enemy";
		}
		eItr++;
	}

	std::map <std::string, friendly*>::iterator fItr = friendlies.begin();
	while (fItr != friendlies.end() )
	{
		if (object == fItr->first)
		{
			return "friendly";
		}
		fItr++;
	}

	std::map <std::string, int>::iterator sItr = specials.begin();
	while (sItr != specials.end())
	{
		if (object == sItr->first)
		{
			return "special";
		}
		sItr++;
	}

	return "null";
}


// If you attack a bot, every bot in the room becomes Hostile
void Room::makeHostile()
{
	std::multimap <std::string, Enemy*>::iterator eItr = enemies.begin();	
	while (eItr != enemies.end())
	{
		Enemy* enemy = eItr->second;
		enemy->setHostile();						// Get each bot in room and set it as hostile
		eItr++;
	}
}

void Room::deactivateEnemies()						// EMP blast kills all enemies in a room
{
	enemies.clear();
}

// Prints out Name, description, pickables, enemies, friendlies and specials in the room
void Room::examine()								
{
	std::cout << "\nI'm in the " << name << std::endl;
	std::cout << description << std::endl;
	std::cout << "There are " << enemies.size() << " " << " Enemies " << std::endl;
	
	//print pickables
	if (!pickables.empty())
	{
		std::cout << " \nI could Take :" << std::endl;

		std::map <std::string, int>::iterator itr = pickables.begin();
		while (itr != pickables.end())
		{
			std::cout << itr->first << std::endl;
			itr++;
		}
	}
	// print friendlies
	if (!friendlies.empty())
	{
		std::cout << "\nI could talk to " << std::flush ;
		std::map <std::string, friendly*>::iterator fItr = friendlies.begin();
		while (fItr != friendlies.end())
		{
			std::cout << fItr->first << std::endl;
			fItr++;
		}
	}

	// print specials
	if (!specials.empty())
	{
		std::cout << "\nI could Activate " << std::flush;
		std::map <std::string, int>::iterator itr = specials.begin();
		while (itr != specials.end())
		{
			std::cout << itr->first << std::endl;
			itr++;
		}		
	}
}