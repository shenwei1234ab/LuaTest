#include <stdio.h>
#include <iostream>
#include "error.h"
extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}


//c++读取Lua测试
void LoadluaTest()
{
	lua_State *L = lua_open();
	luaopen_base(L);
	luaL_openlibs(L);
	luaopen_string(L);
	luaopen_math(L);

	//读取lua文件加入虚拟机
	if (luaL_dofile(L, "F:\\lua\\LuaTest\\project\\src\\loadlua.lua"))
	{
		std::cout << "cannot run configuration file" << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);/* pop error message from the stack */
		return;
	}
	//函数入栈
	lua_getglobal(L, "loadlua");   
	stackDump(L);
	//参数入栈
	lua_pushnumber(L, 6);    /* push 1st argument */
	lua_pushnumber(L, 7);    /* push 2nd argument */
	
	//调用函数，同时会对函数及两个参数进行出栈操作，并压入返回值
	//2:参数个数 1:返回值个数
	lua_call(L, 2, 1);
	//从栈中取回返回值
	int result = lua_tointeger(L, -1);
	//清栈
	lua_pop(L, 1);
	std::cout << result << std::endl;
}



void main1()
{
	LoadluaTest();
	system("pause");
}