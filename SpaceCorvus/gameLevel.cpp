#include "gameLevel.h"
#include "tinyxml2.h"
#include "Weapon.h"
#include "firstAidBox.h"
#include "Electronic.h"
#include "Android.h"
#include "Dreadbot.h"
#include "Ingenoid.h"
#include "Mortroid.h"
#include "Friendly.h"
#include "player.h"
#include <iostream>

using namespace tinyxml2;

gameLevel* gameLevel::instance = NULL;

// Loading game
void gameLevel::initialize()
{
	// Object Factory initialization
	ObjectFactory["weapon"] = Weapon::_createObject;
	ObjectFactory["firstAid"] = firstAidBox::_createObject;
	ObjectFactory["electronic"] = Electronic::_createObject;
	ObjectFactory["special"] = Special::_createObject;

	EnemyFactory["Android"] = Android::_createObject;
	EnemyFactory["Mortroid"] = Mortroid::_createObject;
	EnemyFactory["Ingenoid"] = Ingenoid::_createObject;
	EnemyFactory["Dreadbot"] = Dreadbot::_createObject;
}

void gameLevel::load(XMLElement* rootNode)
{

	// Loading pickable object data     

	XMLElement* pickablesNode = rootNode->FirstChildElement("pickables");				
	if (pickablesNode != NULL)
	{
		XMLElement* pickable = pickablesNode->FirstChildElement("pickable");
		while (pickable != NULL)
		{
			std::string pickableType = pickable->Attribute("type");
			std::map < std::string, std::function<Object*()>> ::iterator it;             // Generates an object depenending on type using fucntion pointer
			it = ObjectFactory.find(pickableType);
			if (it != ObjectFactory.end())
			{
				Object* pickableObj = (*it).second();
				XMLElement* nameNode = pickable->FirstChildElement("name");              // get name of the object  
				
				std::string description;
				std::string objectName  = nameNode->GetText();			
				pickableObj->load(pickable, pickableObj);								 // Load data into the object

				pickableObj->name = objectName;											 // Naming the object

				XMLElement* descriptionNode = pickable->FirstChildElement("description"); // Getting description. This is not done during loading as it can't be accessed by derived function
				if (descriptionNode != NULL)
				{
					description = descriptionNode->GetText();
					pickableObj->description = description;
				}

				addPickable(objectName, pickableObj);									 // Map the object pointer to the name
			}
			pickable=pickable->NextSiblingElement("pickable");
		}
	}


	// loading Specials
	XMLElement* specialsNode = rootNode->FirstChildElement("specials");
	if (specialsNode != NULL)
	{
		XMLElement* specialNode = specialsNode->FirstChildElement("special");
		while (specialNode != NULL)
		{
			Special* special = new Special();
			XMLElement* childNode = specialNode->FirstChildElement("name");
			special->name = childNode->GetText();
			childNode = specialNode->FirstChildElement("description");
			special->description = childNode->GetText();

			addSpecials(special->name, special);
			specialNode = specialNode->NextSiblingElement("special");
		}
	}

	// Loading Friendlies
	XMLElement* friendliesNode = rootNode->FirstChildElement("friendlies");
	if (friendliesNode != NULL)
	{
		XMLElement* friendlyNode = friendliesNode->FirstChildElement("friendly");
		while (friendlyNode != NULL)
		{
			friendly* NPC = new friendly();
			XMLElement* childNode = friendlyNode->FirstChildElement("name");
			NPC->name = childNode->GetText();
			childNode = friendlyNode->FirstChildElement("reward");
			NPC->reward = childNode->GetText();
			childNode = friendlyNode->FirstChildElement("dialog");
			NPC->dialog = childNode->GetText();
			childNode = friendlyNode->FirstChildElement("description");
			NPC->description = childNode->GetText();

			addFriendly(NPC->name, NPC);														// Map friendly object to the friendly name

			friendlyNode=friendlyNode->NextSiblingElement("friendly");
		
		}
	}

	//loading Rooms

	XMLElement* roomsNode = rootNode->FirstChildElement("rooms");
	if (roomsNode != NULL)
	{
		XMLElement* roomNode = roomsNode->FirstChildElement("room");
		while (roomNode != NULL)
		{
			Room* room = new Room();

			//If the room has a node called startRoom then initalize player's startRoom variable
			XMLElement* startRoom = roomNode->FirstChildElement("startRoom");
			if (startRoom != NULL)
			{
				Player::Instance().startRoom = room;
				Player::Instance().currentRoom = room;
			}

			// Loading attributes of the room

			room->name = roomNode->Attribute("name");
			XMLElement* childNode = roomNode->FirstChildElement("description");
			room->description = childNode->GetText();
			childNode = roomNode->FirstChildElement("action");
			room->action = childNode->GetText();
			childNode = roomNode->FirstChildElement("locked");
			std::string locked = childNode->GetText();
			if (locked == "Yes")
				room->locked = true;

			// if a room requires a key card to be opened, it is saved.
			childNode = roomNode->FirstChildElement("keycard");														// getting key card value
			if (childNode != NULL)
			{
				std::string key = childNode->GetText();
				room->keycard = key;
			}

			// loading pickables
			XMLElement* pickables = roomNode->FirstChildElement("pickables");
			if (pickables != NULL)
			{
				XMLElement* objectNode = pickables->FirstChildElement("object");
				while (objectNode != NULL)
				{
					// Adding the pickables name and number
					std::string objectName = objectNode->Attribute("name");
					int number = std::stoi(objectNode->GetText());
					room->pickables[objectName] = number;				

					objectNode = objectNode->NextSiblingElement("object");
				}
			}

			// loading Specials
			XMLElement* special = roomNode->FirstChildElement("special");
			if (special != NULL)
			{
				std::string specialName = special->GetText();
				room->specials[specialName] = 1;
			}


			// loading Enemies
			XMLElement* enemies = roomNode->FirstChildElement("enemies");
			if (enemies != NULL)
			{
				XMLElement* enemyNode = enemies->FirstChildElement("enemy");
				while (enemyNode != NULL)
				{
					// Adding the enemy type and number
					std::string enemyType = enemyNode->Attribute("type");
					int number = std::stoi(enemyNode->GetText());
					for (int i = 0; i < number; i++)
					{
						room->enemies.insert(std::map<std::string, Enemy*>::value_type(enemyType, createEnemy(enemyType)));
					}
					
					enemyNode = enemyNode->NextSiblingElement("enemy");
				}
			}

			//loading waypoints
			XMLElement* waypoints = roomNode->FirstChildElement("waypoint");
			if (waypoints != NULL)
			{
				XMLElement* roomWayNode = waypoints->FirstChildElement("room");
				while (roomWayNode != NULL)
				{
					// Adding the rooms to each direction
					std::string direction = roomWayNode->Attribute("direction");
					std::string roomName;
					if (roomWayNode->GetText() != nullptr)
						roomName = roomWayNode->GetText();
					else
						roomName = "null";

					room->waypoint[direction] = roomName;

					roomWayNode = roomWayNode->NextSiblingElement("room");
				}
			}

			// loading frienldies
			XMLElement* friendlyNode = roomNode->FirstChildElement("friendlies");
			if (friendlyNode != NULL)
			{
				XMLElement* NPCNode = friendlyNode->FirstChildElement("friendly");
				while (NPCNode != NULL)
				{
					std::string NPCname = NPCNode->Attribute("name");
					friendly* NPC  = getFriendly(NPCname);

					//Mapping NPC name and NPC pointer
					room->friendlies[NPCname] = NPC;
					NPCNode = NPCNode->NextSiblingElement("friendly");
				}
			}				
			
			addRoom(room->name, room);
			roomNode = roomNode->NextSiblingElement("room");
		}
	}

	// Creating enemies for Database

	Android* android = new Android;
	addEnemy("Android", android);
	Ingenoid* ingenoid = new Ingenoid;
	addEnemy("Ingenoid", ingenoid);
	Mortroid* mortroid = new Mortroid;
	addEnemy("Mortroid", mortroid);
	Dreadbot* dreadbot = new Dreadbot;
	addEnemy("Dreadbot", dreadbot);

}


// Mapping objects to string and creating a database

void gameLevel::addEnemy(std::string name, Enemy* enemy)      // adding enemy
{
	enemyDB[name] = enemy;
}

void gameLevel::addFriendly(std::string name, friendly* friendly) // adding friendly
{
	friendlyDB[name] = friendly;
}

void gameLevel::addPickable(std::string name, Object* pickable)  // adding pickable
{
	pickablesDB[name] = pickable;
}

void gameLevel::addRoom(std::string name, Room* room) // adding room
{
	roomDB[name] = room;
}

void gameLevel::addSpecials(std::string name, Special* special)
{
	specialsDB[name] = special;
}


// Retrieving object based on the name passed to function

Enemy* gameLevel::getEnemy(std::string name)  // Iterates through the map and returns the enemy pointer
{
	std::map<std::string, Enemy*>::iterator itr = enemyDB.begin();
	while (itr != enemyDB.end())
	{
		if (itr->first == name)
			return itr->second;
		itr++;
	}
	return NULL;
}

Object* gameLevel::getPickable(std::string name)  // Iterates through the map and returns the pickable pointer
{
	std::map<std::string, Object*>::iterator itr = pickablesDB.begin();
	while (itr != pickablesDB.end())
	{
		if (itr->first == name)
			return itr->second;
		itr++;
	}
	return NULL;
}

friendly* gameLevel::getFriendly(std::string name)  // Iterates through the map and returns the friendly pointer
{
	std::map<std::string, friendly *>::iterator itr = friendlyDB.begin();
	while (itr != friendlyDB.end())
	{
		if (itr->first == name)
			return itr->second;
		itr++;
	}
	return NULL;
}

Room* gameLevel::getRoom(std::string name) // returns room pointer
{
	
	std::map<std::string, Room*>::iterator itr = roomDB.begin();
	while (itr != roomDB.end())
	{
		if (itr->first == name)
			return itr->second;
		itr++;
	}
	return NULL;
}

Special* gameLevel::getSpecial(std::string name)	// returns the special object pointer based on name passed as parameter
{
	std::map<std::string, Special*>::iterator itr = specialsDB.begin();
	while (itr!= specialsDB.end())
	{
		if (itr->first == name)
			return itr->second;
		itr++;
	}
	return NULL;
}


Enemy* gameLevel::createEnemy(std::string enemyType)	// Returns a new enemy object based on enemyType passed 
{
	if (enemyType == "Android")
		return new Android;
	if (enemyType == "Ingenoid")
		return new Ingenoid;
	if (enemyType == "Dreadbot")
		return new Dreadbot;
	if (enemyType == "Mortroid");
		return new Mortroid;
}



