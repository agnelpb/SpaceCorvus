#ifndef _PLAYER_H_
#define _PLAYER_H_

#pragma once
#include <map>
#include <string>
#include "Room.h"
#include "Weapon.h"

class Player
{

private:


	static Player* instance;

	inline explicit Player()
	{
		inCombat = false;
	}

	inline ~Player()
	{}

	inline explicit Player(Player const&)
	{}

	inline Player& operator=(Player const&)
	{}


	

public:
	
	
	float health;
	std::list<std::string> inventory;
	Room* currentRoom;
	Room* startRoom;
	Object* currentWeapon;
	bool inCombat;

	static Player& Instance()
	{
		if (instance == NULL)
		{
			instance = new Player();
			Player::Instance().initialize();
		}
		return *instance;
	}

	void initialize();
	void describe();
	void combat(std::string enemy);
	void hostileCheck();
	void talkTo(std::string);
	void goToRoom(Room*);
	Object* findInInventory(std::string);
	void removeFromInventory(std::string);
	void use(std::string);
	void knockedOut();
	void hack(std::string);
	void activate(std::string);
	inline int  getWeaponDamage() { return static_cast<Weapon*>(currentWeapon)->damage; };
	inline void setWeaponDamage(int value) { static_cast<Weapon*>(currentWeapon)->damage = value; };
	void endGame();
};

#endif _PLAYER_H_