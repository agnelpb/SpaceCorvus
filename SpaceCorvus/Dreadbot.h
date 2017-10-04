#ifndef _DREADBOT_H_
#define _DREADBOT_H_
#pragma once

#include "Enemy.h"

class Dreadbot : public Enemy
{
public:

	static inline Enemy* _createObject()
	{
		return new Dreadbot;
	}

	Dreadbot();
	~Dreadbot();
};

#endif _DREADBOT_H_