#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <string>
#include "tinyxml2.h"

using namespace tinyxml2;

class Object
{

public:

	inline std::string getName() { return name; };
	inline std::string getDescription() { return description ; };

private:

	friend class gameLevel;
	friend class Parser;

protected:

	std::string name;
	std::string description;

	Object();
	virtual	~Object();

	
	virtual void load(XMLElement* object, Object* pickableObj) = 0;
};


#endif _OBJECT_H
