#pragma once
class ISystem
{
public:
	inline  ISystem()
	{}

	virtual ~ISystem()
	{}

	friend class GameEngine;

	virtual void initialize() = 0;
};