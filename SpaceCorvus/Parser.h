
#ifndef _PARSER_H_
#define _PARSER_H_
#pragma once

#include <string>
#include "Isystem.h"
#include <map>
#include <functional>

class Parser : public ISystem
{
	
// A function pointer type
typedef void(Parser::*xCommand)(std::string);


private:

	std::string command;
	std::string primaryCommand = "";
	std::string secondaryCommand = "";
	bool invalidCommand = false;

	std::map<const char*, xCommand> commands;	// A map of string to function pointer.

	// Functions for each command
	void examine(std::string);
	void go(std::string);
	void attack(std::string);
	void take(std::string);
	void activate(std::string);
	void hack(std::string);
	void drop(std::string);
	void talk(std::string);
	void use(std::string);
	void help(std::string);
	void list(std::string);
	void save(std::string);
	void load(std::string);
	void quit(std::string);

	void extractCommand(std::string);			
	xCommand findCommand(std::string);
	static Parser* instance;

	inline explicit Parser()
	{}

	inline ~Parser()
	{}

	inline explicit Parser(Parser const&)
	{}

	inline Parser& operator= (Parser const&)
	{}

public:

	static Parser& Instance()
	{
		if (instance == NULL)
		{
			instance = new Parser();
			Parser::Instance().initialize();
		}

		return *instance;
	}

	void parseCommand(std::string command);

	void getCommand();								// Get command
	void combatCommand(std::string enemy);			// Get command when a hostile enemy has spotted you.

protected:


	void initialize();	

};

#endif _PARSER_H_

