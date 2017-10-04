#ifndef _MORTROID_H_
#define _MORTROID_H_

#pragma once

#include "Enemy.h"

class Mortroid : public Enemy
{
	
public:

	static inline Enemy* _createObject()
	{
		return new Mortroid;
	}

	Mortroid();
	~Mortroid();
};

#endif _MORTROID_H_
