#ifndef _GAMEENGINE_H_
#define _GAMEENGINE_H_
#pragma once

#include <map>
#include <string>
#include "Object.h"


class GameEngine
{

private:
	
	static  GameEngine* instance;

	inline explicit GameEngine()
	{}

	inline ~GameEngine()
	{}

	inline explicit GameEngine(GameEngine const&)
	{}

	inline GameEngine& operator= (GameEngine const&)
	{
		return *this;
	}



public:

	inline static GameEngine& Instance()
	{
		if (instance == NULL)
		{
			instance = new GameEngine();
		}
		return *instance;
	}

	void initialize();
	void gameLoop();



};

#endif _GAMEENGINE_H_
