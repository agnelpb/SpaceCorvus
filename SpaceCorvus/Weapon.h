#ifndef _WEAPON_H_
#define _WEAPON_H_
#pragma once

#include "Object.h"

class Weapon :	public Object
{

public :

	static inline Object* _createObject()																
	{																										
		return new Weapon;																		   	 
	}

public:
	
	int damage;
	Weapon();
	~Weapon();

	inline void addDamage(int damageUpgrade){ damage += damageUpgrade; };
	void load(XMLElement* object, Object* pickableObj);

};

#endif _WEAPON_H_

