#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#pragma once

#include "ISystem.h"
#include <string>

class FileSystem : public ISystem
{

private:

	static FileSystem* instance;

	inline explicit FileSystem()
	{
	}


	inline ~FileSystem()
	{
	}

	inline explicit FileSystem(FileSystem const&)
	{
	}

	inline FileSystem& operator= (FileSystem const&)
	{
	}

public:
	
	static FileSystem& Instance()
	{
		if (instance == NULL)
		{
			instance = new FileSystem();
		}
		return *instance;
	}

	void loadGameLevel(std::string fileName);
	void saveFile(std::string fileName);
	void loadFile(std::string fileName);

protected:

	void initialize(){}
		

};

#endif _FILESYSTEM_H_