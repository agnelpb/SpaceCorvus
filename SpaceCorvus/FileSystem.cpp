#include "FileSystem.h"
#include "tinyxml2.h"
#include "gameLevel.h"
#include <iostream>
#include "player.h"

using namespace tinyxml2;

FileSystem* FileSystem::instance = NULL;

// Loading gameLevel
void FileSystem::loadGameLevel (std::string fileName)
{


	XMLDocument* doc = new XMLDocument();
	const char* filename = fileName.c_str();
	XMLError eResult = doc->LoadFile(filename);

	if (eResult == XML_NO_ERROR)								// if no error
	{
		XMLElement* rootNode = doc->FirstChildElement("level"); // Load root node
		if (rootNode != NULL)
		{
			gameLevel::Instance().load(rootNode);
		}
	}
}

// Loading save File

void  FileSystem::loadFile(std::string fileName)
{

	Player::Instance().initialize();
	XMLDocument* saveFile = new XMLDocument();
	XMLError eResult = saveFile->LoadFile(fileName.c_str());
	if (eResult == XML_NO_ERROR)
	{
		XMLElement* player = saveFile->FirstChildElement("Player");				// gets player node
		if (player != NULL)
		{
			std::string name;
			Room* room;

			// Setting Health
			XMLElement* health = player->FirstChildElement("Health");			
			if (health != NULL)
			{
				int hitpoints = std::stoi(health->GetText());
				Player::Instance().health = hitpoints;
			}

			// Setting Current Room

			XMLElement* curRoom = player->FirstChildElement("CurrentRoom");		
			if (curRoom != NULL)
			{
				name = curRoom->GetText();
				room = gameLevel::Instance().getRoom(name);
				Player::Instance().currentRoom = room;
			}

			// Setting Start Room
			XMLElement* startRoom = player->FirstChildElement("StartRoom");		
			if (startRoom != NULL)
			{
				name = startRoom->GetText();
				room = gameLevel::Instance().getRoom(name);
				Player::Instance().startRoom = room;
			}

			// Setting Weapon
			XMLElement* weapon = player->FirstChildElement("Weapon");			
			if (weapon != NULL)
			{
				name = weapon->GetText();
				Object* curWeapon = gameLevel::Instance().getPickable(name);
				Player::Instance().currentWeapon = curWeapon;
				int damage = std::stoi(weapon->Attribute("damage"));
				Player::Instance().setWeaponDamage(damage);
			}

			// Setting Inventory
			XMLElement* inventory = player->FirstChildElement("Inventory");		// Start Inventory
			if (inventory != NULL)
			{
				XMLElement* pickable = inventory->FirstChildElement("Pickable");
				while (pickable != NULL)
				{
					name = pickable->GetText();
					if (name != "Pulse Rifle")
						Player::Instance().inventory.push_back(name);
					pickable = pickable->NextSiblingElement("Pickable");
				}
			}
		}
		std::cout << " Successfully Loaded " << fileName.c_str() << std::endl;
	}
	else
		std::cout << " Sorry ! Wrong file name " << fileName.c_str() << std::endl;

	

}

// saving progress

void  FileSystem::saveFile(std::string fileName)
{

	XMLDocument* savefile = new XMLDocument();												// creating save file
	XMLElement* player = savefile->NewElement("Player");									// Adding Player Node
	savefile->InsertEndChild(player);														// Inserting to doc

	XMLElement * health = player->GetDocument()->NewElement("Health");					    // New node to Player Node
	player->InsertEndChild(health);
	health->SetText(Player::Instance().health);												// Adding text to node

	XMLElement* curRoom = player->GetDocument()->NewElement("CurrentRoom");
	player->InsertEndChild(curRoom);
	std::string roomName = Player::Instance().currentRoom->name;
	curRoom->SetText(roomName.c_str());

	XMLElement * curWeapon = player->GetDocument()->NewElement("Weapon");
	player->InsertEndChild(curWeapon);
	curWeapon->SetText(Player::Instance().currentWeapon->getName().c_str());
	curWeapon->SetAttribute("damage", Player::Instance().getWeaponDamage());

	XMLElement * startRoom = player->GetDocument()->NewElement("StartRoom");
	player->InsertEndChild(startRoom);
	startRoom->SetText(Player::Instance().startRoom->name.c_str());

	XMLElement * inventory = player->GetDocument()->NewElement("Inventory");
	player->InsertEndChild(inventory);

	std::string pickObj;
	std::list<std::string>::iterator itr = Player::Instance().inventory.begin();		    // Searching in inventory
	while (itr != Player::Instance().inventory.end())
	{
		XMLElement* pickable = player->GetDocument()->NewElement("Pickable");				// Adding each item in inventory as pickable
		inventory->InsertEndChild(pickable);
		pickable->SetText((*itr).c_str());
		itr++;
	}

	savefile->SaveFile(fileName.c_str());
	std::cout << " Progress Saved ! " << std::endl;

}