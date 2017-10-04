#include "Player.h"
#include <iostream>
#include <string>
#include "gameLevel.h"
#include "time.h"
#include "Parser.h"



Player* Player::instance = NULL;


void Player::initialize()
{
	if (!inCombat)															// Preventing losing the weapon after getting knocked out
	{																	    // If first time, clean up, add Pulse Rifle as default weapon and assign it as current weapon
		inventory.clear();
		inventory.push_back("Pulse Rifle");
		currentWeapon = gameLevel::Instance().getPickable("Pulse Rifle");
	}
	health = 100;	
	inCombat = false;	
}

void Player::describe()
{
	std::cout << currentRoom->description << std::endl;						// Describing the room
	if (!currentRoom->visited)                                              // If entering a room for the first time, describe the scenatrio and possible actions to take
	{
		std::cout << currentRoom->action << std::endl;
		currentRoom->visited = true;
	}		
}

// C O M B A T 
void Player::combat(std::string enemy)
{
	inCombat = true;
	std::multimap <std::string, Enemy*>::iterator eItr = currentRoom->enemies.begin();						// Finding out the bot in the room					
	while (eItr != currentRoom->enemies.end())
	{
		if (eItr->first == enemy)
		{
			Enemy* enemyBot = eItr->second;
			std::cout << "Firing "<< currentWeapon->getName() << " at " << enemy << std::endl;
			enemyBot->reduceHealth(static_cast<Weapon*>(currentWeapon)->damage);								// Reduces damage of weapon from health of bot
			_sleep(1000);
			
			if (enemyBot->getHealth() <= 0)
			{
				currentRoom->enemies.erase(eItr);															// Removing the dead bot
				std::cout << enemy << " destroyed ! " << std::endl;
				std::cout << "Player Health : " << health << std::endl;
				inCombat = false;
			}
			else
			{
				std::cout << enemy << " attacking... " << std::endl;
				Player::Instance().health -= enemyBot->getDamage();											// Reduces player health
				_sleep(1000);
				if (health < 0)
					health = 0;
				std::cout << "Enemy Health : " << enemyBot->getHealth() << std::flush;
				std::cout << " \t \t Player Health : " << health << std::endl;

				if (health <= 0)                                                                                // Player Dead
				{
					knockedOut();					
					return;
				}

				Parser::Instance().combatCommand(enemy);
			}			
			break;
		}
		if(eItr!=currentRoom->enemies.end()) eItr++;															// Iterate until you find the bot you are trying to attack
	}	
}


void Player::hack(std::string enemyName)
{
	Object* hackdex = findInInventory("HackDex");																
	if (hackdex == NULL)
	{
		std::cout << " You need a HackDex to hack it " << std::endl;										// If you don't have HackDex, you don't hack
		return;
	}

	std::multimap <std::string, Enemy*>::iterator eItr = currentRoom->enemies.begin();						// Finding out the bot in the room					
	while (eItr != currentRoom->enemies.end())
	{
		if (eItr->first == enemyName)
		{
			if (enemyName == "Dreadbot")																	
			{
				std::cout << " Hacking Dreadbot... " << std::endl;
				_sleep(1000);
				std::cout << " Dreadbot auto destruct sequence initiated... " << std::endl;
				_sleep(1000);
				std::cout << " Dreadbot destroyes itself.." << std::endl;
				currentRoom->enemies.erase(eItr);															// If Dreadbot, destroy it
				inCombat = false;
			}
			else
			{
				if (!eItr->second->isHacked())																// Only hackable once
				{
					std::cout << " Hacking Mortroid... " << std::endl;
					_sleep(1000);
					std::cout << " Mortroid attack strength reduced.... " << std::endl;
					_sleep(1000);
					eItr->second->reduceDamage(40);															// If Mortroid, reduce it's attack strength and set it as hacked
					eItr->second->hacked();
				}
				else
				{
					std::cout << " Mortroid already hacked ! " << std::endl;
				}
			}			
			return;
		}
		eItr++;
	}
	std::cout << " There is no " << enemyName << " in this room ! " << std::endl;
}

void Player::knockedOut()
{
	bool dropable;
	std::list<std::string>::iterator itr = inventory.begin();									// droping the items in inventory
	std::list<std::string>::iterator tempItr;

	while (itr != inventory.end())
	{
		dropable = false;
		if ((*itr) != currentWeapon->getName()) // && (*itr).find("Key")==std::string::npos)	// Won't drop the weapon
		{
			dropable = true;
			tempItr = itr;
			currentRoom->pickables[*itr] = 1;													// dropping to room
		}
		itr++;
		if (dropable)
			inventory.erase(tempItr);																// Removing from inventory
	}	
	std::cout << "\n\n You were knocked out ! !\n\n You woke up in the " << startRoom->name << std::endl;
	std::cout << " You lost all your items in the inventory. You can scavenge them back from " << currentRoom->name << std::endl;
	currentRoom = startRoom;																	// Player spawning
	initialize();																				// Initializing

}

// Checking if hostile enemies, enter combat
void Player::hostileCheck()
{	
	if (!currentRoom->enemies.empty())																	// if there are enemies
	{
		std::multimap <std::string, Enemy*>::iterator eItr = currentRoom->enemies.begin();				
		while (eItr != currentRoom->enemies.end())
		{
			Enemy* enemyBot = eItr->second;
			if (enemyBot->isHostile())
			{
				std::cout << "A hostile " << enemyBot->getName() << " spotted you !  You could Attack it or Hack it." <<std::endl;
				Parser::Instance().combatCommand(enemyBot->getName());										// Get player command
				break;
			}
			eItr++;
		}
	}
}


void Player::talkTo(std::string friendlyName)
{
	if (!currentRoom->friendlies.empty())
	{
		std::multimap <std::string, friendly*>::iterator fItr = currentRoom->friendlies.begin();
		while (fItr != currentRoom->friendlies.end())
		{
			if (fItr->first == friendlyName)
			{
				// Get the friendly object 
				friendly* friendly = fItr->second;
				std::cout << friendly->dialog << std::endl;
				
				// If first time visiting
				if (!friendly->visited)
				{
					inventory.push_back(friendly->reward);												// Get reward for talking
					if (friendlyName == "IT Specialist")												// IT Specialist has a weapons upgrade.
						static_cast<Weapon*>(currentWeapon)->addDamage(20);
					
					if (friendlyName == "Guard Commander")
					{
						Player::Instance().currentWeapon = findInInventory(friendly->reward);			// Set Neutralizer as new weapon
					}
				}
				friendly->visited = true;

				// If Quest not completed
				if (!friendly->questCompleted)
				{
					if (friendlyName == "Ambro")
					{
						Object* battery = findInInventory("Battery Pack");										
						Object* base = findInInventory("Base Module");
						Object* zig = findInInventory("ZigBee Module");
						Object* ic = findInInventory("IC Chip");

						if (battery != NULL && base != NULL && zig != NULL && ic != NULL)				// If player has the four components, 
						{
							removeFromInventory("Battery Pack");										// delete from inventory
							removeFromInventory("Base Module");
							removeFromInventory("ZigBee Module");
							removeFromInventory("IC Chip");

							inventory.push_back("HackDex");												// and add hackDex to inventory
							std::cout << " \n Here is the HackDex as I promised. Hacked enemies won't attack you. You can only hack the Dreadbots or Mortroids with it." << std::endl;
							friendly->questCompleted = true;
						}
					}

					if (friendlyName == "Captain")
					{
						Special* lThrust = gameLevel::Instance().getSpecial("Left Thruster");
						Special* rThrust = gameLevel::Instance().getSpecial("Right Thruster");
						Special* door = gameLevel::Instance().getSpecial("Blast Door Console");
						Special* core = gameLevel::Instance().getSpecial("Quantum Core Console");

						if (lThrust->activated && rThrust->activated && door->activated && core->activated)        // If player has activated all four consoles
						{
							std::cout << " \n Good ! You have activated them. Ambro had revoked the access card to Nav Deck. Here is the new key card to Nav Deck. \nGo to Nav Deck and enable shields and naviagtion systems. " << std::endl;
							inventory.push_back("Key Card NavDeck");
							friendly->questCompleted = true;
						}
					}

					if (friendlyName == "Nav Officer")
					{
						Special* navCon = gameLevel::Instance().getSpecial("Navigation Console");				// If Navigation console activated
						if (navCon->activated)
						{
							std::cout << "\nYou did it ! We have regained the ship's navigation controls. We are being re-routed to safe location.\nThank you for helping us ! Captain sends his regards. " << std::endl;
							endGame();																			// Show credits
						}
					}
				}
				break;
			}
			fItr++;
		}
	}
}


void Player::goToRoom(Room* newRoom)
{
	if (newRoom->locked)																	 // If room is locked
	{
		Object* key = findInInventory(newRoom->keycard);									 // checking if you have keycard
		if (key == NULL)
		{
			std::cout << "You need " << newRoom->keycard << " to have access to " << newRoom->name << std::endl;
			return;
		}
		else
			newRoom->locked = false;														// unlock door with keycard
	}

	if (newRoom->name == "Med Bay")															 // After you die, you respawn in Med Bay
		startRoom = newRoom;

	currentRoom = newRoom;																	 // Set new room as the current room and describe it
	describe();
}


Object* Player::findInInventory(std::string objectName)
{
	std::list<std::string>::iterator itr = inventory.begin();								 // Searching in inventory
	while (itr != inventory.end())
	{
		if ((*itr) == objectName)
		{
			Object* object = gameLevel::Instance().getPickable(objectName);					 // Return the object
			return object;
		}
		itr++;
	}
	return NULL;
}

// Removes an object from inventory
void Player::removeFromInventory(std::string objectName)
{
	std::list<std::string>::iterator itr = inventory.begin();								 
	while (itr != inventory.end())
	{
		if ((*itr) == objectName)
		{
			inventory.erase(itr);
			break;
		}
		itr++;
	}
}

void Player::use(std::string object)
{
	Object* obj = findInInventory(object);						// If in inventory
	if (obj != NULL)
	{
		if (obj->getName() == "PainKiller")						// If Painkillers add health 
		{
			if (health == 100)
			{
				std::cout << "No, Thank You. I'm perfectly healthy. " << std::endl;
				return;
			}
			health += 50;		
			if (health > 100)
				health = 100;
			removeFromInventory(object);							// and remove it from inventory.
			std::cout << " You took a PainKiller. Your health has been restored." << std::endl;
			return;
		}

		if (obj->getName() == "EMP Grenade")				
		{
			Player::Instance().currentRoom->deactivateEnemies();	// Kill all enemies
			std::cout << " The EMP wave destroyed the enemies in the room ! Your electronic implants are buzzing... Maybe you shouldn't do this often..." << std::endl;
			removeFromInventory(object);
			inCombat = false;
			Player::Instance().health -= 20;						// Get out of combat mode and reduce player health
			return;
		}

		if (obj->getName() == "Neutralizer")					       // Equip weapon
		{
			Player::Instance().currentWeapon = obj;
			std::cout << " Neutralizer Equiped ! " << std::endl;
		}
	}
	else
		std::cout << " Can't find " << object << " in the inventory !" << std::endl;

}


void Player::activate(std::string object)
{
	Object* navPad = findInInventory("Nav Controller");								// Getting Nav controller for later use
	std::string objType = currentRoom->findObjectType(object);
	if (objType == "special")													    
	{
		Special* special = gameLevel::Instance().getSpecial(object);				// Get the special object
		if (!special->activated)												    // If not activated
		{
			if (special->getName() == "Navigation Console" && navPad == NULL)		// If It is nav console and you don't have the Navigation Controller
			{
				std::cout <<" You need the Nav Controller to activate it ! " << std::endl;
				return;
			}
			special->activated = true;												// else, you can activate the object
			std::cout << object << " Activated. " << std::endl;
		}
		else
			std::cout << object << " has already been activated. " << std::endl;
	}
	else
		std::cout << object << " ? You can't activate that ! " << std::endl;
}

// Credit Scene
void Player::endGame()
{
	_sleep(2000);
	std::cout << " \n\n\n\t\t\tTHANK YOU FOR PLAYING SPACE CORVUS\n " << std::endl;
	_sleep(2000);
	std::cout << "\t\t\tA text-adventure game by Agnel Blaise.\n\t\t\tGet more games at : www.agnelpb.com\n " << std::endl;
	_sleep(2000);
}