#ifndef _FRIENDLY_H_
#define _FRIENDLY_H_

#pragma once

#include <string>
#include <list>

class friendly 
{

private: 

	

public:
	
	std::string name;
	std::string description;
	std::string reward;
	std::string dialog;
	bool questCompleted;
	bool visited;
	bool rewarded;
	
	friendly();
	~friendly();
};


#endif _FRIENDLY_H_
