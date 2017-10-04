#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <string>

#pragma once
class Enemy
{

private : 
		

public:


	inline bool isHacked() { return botHacked; };
	inline int getHealth() { return health; };
	inline bool isActivated() { return activated; };
	inline std::string getName() { return name; };
	inline std::string getDescription() { return description; };
	inline int getDamage() { return damage; };
	inline bool isHostile(){ return hostile; };

	inline void reduceHealth(int rHealth){ health -= rHealth; };
	inline void reduceDamage(int rDamage){ damage -= rDamage; };				// Reduce damage by hacking
	inline void hacked(){ botHacked = true; };
	inline void setHostile() { hostile = true; };


protected:

	
	int health;
	int damage;
	std::string name;
	std::string description;
	bool activated;
	bool botHacked;
	bool hostile;

	Enemy();
	~Enemy();
};

#endif _ENEMY_H_