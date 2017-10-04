#ifndef _ROOM_H_
#define _ROOM_H_

#pragma once
#include <map>
#include <list>
#include <string>
#include "friendly.h"
#include "Enemy.h"

class Room
{



public:
	Room();
	~Room();


	std::string name;
	std::string description;
	std::string action;
	std::string keycard;
	bool visited;
	bool locked;
	std::map <std::string, int> pickables;
	std::map<std::string, int> specials;
	std::map <std::string, std::string> waypoint;
	std::multimap <std::string, Enemy* > enemies;
	std::map <std::string, friendly*> friendlies;

	std::string findObjectType(std::string object);

	void makeHostile();
	void examine();
	void deactivateEnemies();

private:

	

};

#endif _ROOM_H_