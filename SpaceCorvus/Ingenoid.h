#ifndef _INGENOID_H_
#define _INGENOID_H_
#pragma once

#include  "Enemy.h"

class Ingenoid : public Enemy
{

private:


public:

	static inline Enemy* _createObject()
	{
		return new Ingenoid;
	}

	Ingenoid();
	~Ingenoid();
};

#endif _INEGNOID_H_