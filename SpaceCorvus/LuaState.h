///-------------------------------------------------------------------------------------------------
// file:	LuaState.h
//
// summary:	Declares the lua state class
//
//			Copyright (c) 2013 Aegis. All rights reserved.
//
//			Date		Developer		Change
//			28/01/2013	William Barry	Created
///-------------------------------------------------------------------------------------------------
#pragma once

#include "lua.hpp"
#include <vector>

///-------------------------------------------------------------------------------------------------
/// <summary>	Defines an alias representing the *l. </summary>
///
/// <remarks>	William Barry, 27/01/2013. </remarks>
///-------------------------------------------------------------------------------------------------
typedef int (*luaCallBackFunction) (lua_State *L);

///-------------------------------------------------------------------------------------------------
/// <summary>	Lua state. </summary>
///
/// <remarks>	William Barry, 27/01/2013. </remarks>
///-------------------------------------------------------------------------------------------------
class LuaState
{
public:
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Default constructor. </summary>
	///
	/// <remarks>	William Barry, 27/01/2013. </remarks>
	///-------------------------------------------------------------------------------------------------
	LuaState();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Destructor. </summary>
	///
	/// <remarks>	William Barry, 27/01/2013. </remarks>
	///-------------------------------------------------------------------------------------------------
	~LuaState(void);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets lua state. </summary>
	///
	/// <remarks>	William Barry, 27/01/2013. </remarks>
	///
	/// <returns>	null if it fails, else the lua state. </returns>
	///-------------------------------------------------------------------------------------------------
	static LuaState* getLuaState()
	{ 
		if (_luaState == NULL)
		{
			_luaState = new LuaState();
		}
		return _luaState; 
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Registers the function. </summary>
	///
	/// <remarks>	William Barry, 27/01/2013. </remarks>
	///
	/// <param name="_functionName">	Name of the function. </param>
	/// <param name="_func">			The function. </param>
	///-------------------------------------------------------------------------------------------------
	void registerFunction(const char* _functionName, luaCallBackFunction _func);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Executes the lua file operation. </summary>
	///
	/// <remarks>	William Barry, 27/01/2013. </remarks>
	///
	/// <param name="_filename">	Filename of the file. </param>
	///-------------------------------------------------------------------------------------------------
	void executeLuaFile(const char* _filename);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Executes the lua string operation. </summary>
	///
	/// <remarks>	William Barry, 27/01/2013. </remarks>
	///
	/// <param name="_filename">	Filename of the file. </param>
	///-------------------------------------------------------------------------------------------------
	void executeLuaString(const char* _filename);

private:
	lua_State *mLuaState;
	static LuaState* _luaState;
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Lua register. </summary>
///
/// <remarks>	William Barry, 27/01/2013. </remarks>
///-------------------------------------------------------------------------------------------------
class LuaRegister
{
public:
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Constructor. </summary>
	///
	/// <remarks>	William Barry, 27/01/2013. </remarks>
	///
	/// <param name="_functionCall">	The function call. </param>
	/// <param name="_function">		The function. </param>
	///-------------------------------------------------------------------------------------------------
	LuaRegister(const char* _functionCall, luaCallBackFunction	_function)
	{
		LuaState::getLuaState()->registerFunction(_functionCall, _function);
	};
};

///-------------------------------------------------------------------------------------------------
/// <summary>	A macro that defines how to register lua function. </summary>
///
/// <remarks>	William Barry, 27/01/2013. </remarks>
///
/// <param name="className">   	Name of the class. </param>
/// <param name="functionName">	Name of the function. </param>
///-------------------------------------------------------------------------------------------------
#define REGISTER_LUA_FUNCTION(className, functionName)							\
	static LuaRegister className##_mLuaCall_##functionName (#functionName, className##_##functionName);
