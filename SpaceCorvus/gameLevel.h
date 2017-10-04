
#ifndef _GAMELEVEL_H_
#define _GAMELEVEL_H_

#pragma once

#include <map>
#include <string>
#include "Object.h"
#include "Room.h"
#include "tinyxml2.h"
#include <functional>
#include "Enemy.h"
#include "Android.h"
#include "Dreadbot.h"
#include "Ingenoid.h"
#include "Mortroid.h"
#include "Special.h"


class gameLevel
{

private:


	static gameLevel* instance;
	std::map<std::string, std::function<Object*()>> ObjectFactory;        // function pointer to different types of objects
	std::map<std::string, std::function<Enemy*()>> EnemyFactory;

	inline explicit gameLevel()
	{}

	inline ~gameLevel()
	{}

	inline explicit gameLevel(gameLevel const&)
	{}

	inline gameLevel& operator= (gameLevel const&)
	{}

	// Database that store an object of every type of enemy, friendly, objects, specials and room
	// Used for referencing using string.
	std::map<std::string, Enemy*> enemyDB;
	std::map<std::string, friendly*> friendlyDB;
	std::map<std::string, Object*> pickablesDB;
	std::map<std::string, Special*> specialsDB;
	std::map<std::string, Room*> roomDB;

	


public:

	static gameLevel& Instance()
	{
		if (instance == NULL)
		{
			instance = new gameLevel();
			instance->initialize();                       // Initializing the game level
		}
		return *instance;
	}

	// Functions to add objects the database
	void addEnemy(std::string name, Enemy* enemy);
	void addFriendly(std::string name, friendly* friendly);
	void addPickable(std::string name, Object* pickable);
	void addSpecials(std::string name, Special* special);
	void addRoom(std::string name, Room* room);

	// functions to retrieve objects from database by their name
	Enemy* getEnemy(std::string name);
	friendly* getFriendly(std::string name);
	Object* getPickable(std::string name);
	Special* getSpecial(std::string name);
	Room* getRoom(std::string name);

	std::string getDescription(std::string name);

	void load(XMLElement* rootNode);

	void initialize();

	Enemy* createEnemy(std::string);	
	
};




#endif _GAMELEVEL_H_