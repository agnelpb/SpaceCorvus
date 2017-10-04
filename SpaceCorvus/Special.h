#pragma once
#include "Object.h"
class Special :	public Object
{
	
public:

	static inline Object* _createObject()
	{
		return new Special;
	}

	void load(XMLElement* object, Object* pickableObj);

	bool activated;

	Special();
	~Special();
};

