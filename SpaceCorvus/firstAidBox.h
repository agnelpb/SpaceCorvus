#ifndef _FIRSTAIDBOX_H_
#define _FIRSTAIDBOX_H_

#pragma once
#include "Object.h"

class firstAidBox :	public Object
{
public:
	
	static inline Object* _createObject()
	{
		return new firstAidBox;
	}


	int health;
	firstAidBox();
	~firstAidBox();

	void load(XMLElement* object, Object* pickableObj);

};


#endif _FIRSTAIDBOX_H_
