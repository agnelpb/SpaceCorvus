#ifndef _ANDROID_H_
#define _ANDROID_H_
#pragma once

#include "Enemy.h"

class Android : public Enemy
{
public:

	static inline Enemy* _createObject()
	{
		return new Android;
	}


	Android();
	~Android();
};

#endif _ANDROID_H_