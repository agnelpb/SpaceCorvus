#include "Parser.h"
#include "Player.h"
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include "gameLevel.h"
#include "FileSystem.h"

Parser* Parser::instance = NULL;



void Parser::initialize()
{
	
	// Mapping each command string to a member fucntion
	commands.insert(std::map<const char*, xCommand>::value_type("Examine", &Parser::examine));
	commands.insert(std::map<const char*, xCommand>::value_type("Attack", &Parser::attack));
	commands.insert(std::map<const char*, xCommand>::value_type("Talk", &Parser::talk));
	commands.insert(std::map<const char*, xCommand>::value_type("Go", &Parser::go));
	commands.insert(std::map<const char*, xCommand>::value_type("Take", &Parser::take));
	commands.insert(std::map<const char*, xCommand>::value_type("Drop", &Parser::drop));
	commands.insert(std::map<const char*, xCommand>::value_type("List", &Parser::list));
	commands.insert(std::map<const char*, xCommand>::value_type("Use", &Parser::use));
	commands.insert(std::map<const char*, xCommand>::value_type("Save", &Parser::save));
	commands.insert(std::map<const char*, xCommand>::value_type("Load", &Parser::load));
	commands.insert(std::map<const char*, xCommand>::value_type("Help", &Parser::help));
	commands.insert(std::map<const char*, xCommand>::value_type("Quit", &Parser::quit));
	commands.insert(std::map<const char*, xCommand>::value_type("Hack", &Parser::hack));
	commands.insert(std::map<const char*, xCommand>::value_type("Activate", &Parser::activate));

}

// Gets a command from player
void Parser::getCommand()
{
	std::cout << "\n > " << std::flush;
	std::getline(std::cin, command);
	Parser::Instance().parseCommand(command);
}

// Command during combat
void Parser::combatCommand(std::string enemy)
{
	Player::Instance().inCombat = true;
	std::cout << "\n > " << std::flush;
	std::getline(std::cin, command);

	extractCommand(command);
	
	xCommand comPtr = findCommand(primaryCommand);			// searching for command
	if (!invalidCommand)
		(Parser::Instance().*comPtr)(secondaryCommand);

	if (invalidCommand)										// If invalid command, request player for another command
	{
		std::cout << " Invalid Command ! Try saying ' Help Me ' " << std::endl;
		invalidCommand = false;
		combatCommand(enemy);
	}
}

// Command Parsing
void Parser::parseCommand(std::string command)
{	
	if (command == "Help")										// If Help or Quit directly call , else parse Command				
	{
		help(" ");
		return;
	}
	else if (command == "Quit")
	{
		quit(" ");
		return;
	}
	extractCommand(command);									// Splits up words

	if (!invalidCommand)
	{
		xCommand comPtr = findCommand(primaryCommand);			// searching for command
		if (!invalidCommand)
			(Parser::Instance().*comPtr)(secondaryCommand);		// The member function of class instance is called and second word is passed as arg
	}

	if (invalidCommand)
	{
		std::cout << "Invalid Command" << std::endl;
		invalidCommand = false;
	}
}

// Finding command by iterating throught the command map
Parser::xCommand Parser::findCommand(std::string command)
{
	xCommand comPtr;
	std::map<const char*, xCommand>::iterator itr = commands.begin();		// searchign for command in the map
	while (itr != commands.end())
	{
		if (primaryCommand == itr->first)
		{
			xCommand comPtr = itr->second;							   // Stores the corresponding member function pointer and returns it
			return comPtr;
		}
		itr++;
	}
	if (itr == commands.end())											// if not found
	{
		invalidCommand = true;
	}
	return NULL;
}


void Parser::extractCommand(std::string commandText)
{
	//Extract first word
	std::istringstream iss{ commandText };				// Gets first command
	iss >> primaryCommand;

	std::ostringstream oss;							// gets the rest of it and removes space

	oss << iss.rdbuf();
	secondaryCommand = oss.str();
	if (secondaryCommand == "" || secondaryCommand == " ")
		invalidCommand = true;
	else
	{
		secondaryCommand.erase(secondaryCommand.begin());
		//secondaryCommand.erase(std::remove_if(secondaryCommand.begin(), secondaryCommand.end(), std::isspace), secondaryCommand.end());
	}
}


// Command : Examine 

void Parser::examine(std::string object)
{

	if (object == "Room")
	{		
		Player::Instance().currentRoom->examine();
	}
	else
	{
		Room* room = Player::Instance().currentRoom;
		std::string objectType = room->findObjectType(object);					// Find type of object in room and get their description
		if (objectType != "null")
		{
			if (objectType == "pickable")
			{
				Object* pickable = gameLevel::Instance().getPickable(object);
				std::cout << pickable->description << std::endl;
			}
			else if (objectType == "enemy")
			{
				Enemy* enemy = gameLevel::Instance().getEnemy(object);
				std::cout << enemy->getDescription() << std::endl;
			}
			else if (objectType == "friendly")
			{
				friendly* friendly = gameLevel::Instance().getFriendly(object);
				std::cout << friendly->description << std::endl;
			}
			else
			{
				Special* special = gameLevel::Instance().getSpecial(object);
				std::cout << special->description << std::endl;
			}
		}
		else																		// search for object in inventory
		{
			Object* newObj = Player::Instance().findInInventory(object);			// return false if not found
			if (newObj != NULL)
				std::cout << newObj->description << std::endl;
			else
				std::cout << "Examine what ? " << std::endl;
		}
	}	
	
}

// Command : Attack

void Parser::attack(std::string object)
{
	Room* room = Player::Instance().currentRoom;
	room->makeHostile();
	std::string objectType = room->findObjectType(object);					// If the object is enemy
	if (objectType == "enemy")
	{
		Player::Instance().combat(object);									// start attacking
	}
	else
	{
		std::cout << "Attack who ? " << std::endl;
	}

}


// Command : Talk

void Parser::talk(std::string object)
{
	// If in combat, you can't talk
	if (Player::Instance().inCombat)
	{
		std::cout << " You are in a combat ! You can't do that ! " << std::endl;
		return;
	}

	// If friendly, talk
	Room* room = Player::Instance().currentRoom;
	std::string objectType = room->findObjectType(object);
	if (objectType == "friendly")
	{
		Player::Instance().talkTo(object);
	}
	else
	{
		std::cout << " Ummm... there is no " << object << " here..."<< std::endl;
	}
}

// Command : Go

void Parser::go(std::string direction)
{

	if (Player::Instance().inCombat)
	{
		std::cout << " You are in a combat ! You can't do that ! " << std::endl;
		return;
	}

	// Find the room pointer by searching the waypoints in a room with direction as arg
	Room* room = Player::Instance().currentRoom;
	std::map <std::string, std::string>::iterator rItr = room->waypoint.begin();
	while (rItr != room->waypoint.end())
	{
		if (direction == rItr->first)												
		{
			std::string nextRoom = rItr->second;
			Room* newRoom = gameLevel::Instance().getRoom(nextRoom);
			if (newRoom != nullptr )									// If room exists in database, go
			{
				Player::Instance().goToRoom(newRoom);
			}
			else
			{
				std::cout << "There is nothing there..." << std::endl;
			}
			return;
		}
		rItr++;
	}

		std::cout << "You can only go North, South, East, West, Up or Down" << std::endl;
}


// Command : Take

void Parser::take(std::string object)
{
	// If the object exist in the room, take it
	Room* room = Player::Instance().currentRoom;
	std::map <std::string, int>::iterator pItr = room->pickables.begin();						// Searching for object in current room
	while (pItr != room->pickables.end())
	{
		if (object == pItr->first)																// If found in room
		{
			Player::Instance().inventory.push_back(object);										// add to inventory							// Add to inventory				
			room->pickables.erase(pItr);														// Remove from Room
			std::cout  << object << " added to inventory " << std::endl;
			return;
		}
		pItr++;																		
	}

	std::cout << "I can't find " << object <<" in the room " <<std::endl;
}



// Command : Drop

void Parser::drop(std::string object)
{
	if (object == Player::Instance().currentWeapon->name)						// Can't drop your weapon
		std::cout << " Are you kidding me ? I need that ! " << std::endl;
	else
	{
		Object* newObj = Player::Instance().findInInventory(object);			// return false if not found
		if (newObj != NULL)
		{
			Player::Instance().inventory.remove(object);						// Removing from inventory
			Player::Instance().currentRoom->pickables[object] = 1;				// Adding to current room 
			std::cout << " Dropped " << object << " in the room. " << std::endl;
		}
		else
			std::cout << object << " ?. Can't drop what I don't have.... " << std::endl;
	}
}


// Command : List

void Parser::list(std::string object)
{
	if (object == "Inventory")
	{
		std::cout << "The Inventory conatins : " << std::endl;
		std::list<std::string>::iterator itr = Player::Instance().inventory.begin();									// Searching in inventory
		while (itr != Player::Instance().inventory.end())
		{
			std::cout << (*itr) << std::endl;
			itr++;
		}
	}
	else
		std::cout << " Try saying ' List Inventory '... It works every time !  " << std::endl;

}


// Command : use

void Parser::use(std::string object)
{
	Player::Instance().use(object);
}

// Command : Save

void Parser::save(std::string fileName)
{
	if (Player::Instance().inCombat)
	{
		std::cout << " You are in a combat ! You can't do that ! " << std::endl;
		return;
	}
	FileSystem::Instance().saveFile(fileName);
}


// Command : Load


void Parser::load(std::string fileName)
{
	if (Player::Instance().inCombat)
	{
		std::cout << " You are in a combat ! You can't do that ! " << std::endl;
		return;
	}
	FileSystem::Instance().loadFile(fileName);
}


// Command : Hack

void Parser::hack(std::string enemyName)
{
	if (enemyName == "Dreadbot" || enemyName == "Mortroid")
	{
		Player::Instance().hack(enemyName);		
	}
	else
		std::cout << " You can hack only Dreadbots or Mortroids" << std::endl;
}

// Command : Activate

void Parser::activate(std::string object)
{
	if (Player::Instance().inCombat)
	{
		std::cout << " You are in a combat ! You can't do that ! " << std::endl;
		return;
	}
	Player::Instance().activate(object);
}

// Command : Help

void Parser::help(std::string fileName)
{
	std::cout << "You could use the commands : " << std::endl;
	std::map<const char*, xCommand>::iterator itr = commands.begin();
	while (itr != commands.end())
	{
		std::cout << itr->first << std::endl;
		itr++;
	}
}


// Command : Quit

void Parser::quit(std::string fileName)
{
	if (Player::Instance().inCombat)
	{
		std::cout << " You are in a combat ! You can't do that ! " << std::endl;
		return;
	}

	std::cout << " Unsaved progress will be lost ! Are you really abandoning the ship ? [Y/N]" << std::endl;
	std::string choice;
	std::getline(std::cin, choice);
	if (choice == "Y")
		std::exit(EXIT_SUCCESS);
}

