#ifndef _ELECTRONIC_H_
#define _ELECTRONIC_H_
#pragma once

#include "Object.h"

class Electronic :	public Object
{
public:

	static inline Object* _createObject()
	{
		return new Electronic;
	}

	Electronic();
	~Electronic();
	

	void load(XMLElement* object, Object* pickableObj);
};

#endif _ELECTRONIC_H_