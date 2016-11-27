#pragma once
#include "error.h" 
#include <stdlib.h>
#include <iostream>


//读取config.lua中的全局变量
void load(char *filename, int *width, int *height)
{
	lua_State *L = lua_open();
	luaopen_base(L);
	luaL_openlibs(L);
	luaopen_string(L);
	luaopen_math(L);
	//读取lua文件并压入lua栈
	if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
	{
		std::cout << "cannot run configuration file" << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);/* pop error message from the stack */
		return;
	}
	//将全局变量压入栈中
	lua_getglobal(L, "width");		//-2
	lua_getglobal(L, "height");		//-1栈顶


	//读取栈上的元素(index)
	if (!lua_isnumber(L, -2))
	{
		luaL_error(L, "`width' should be a number\n");
	}

	if (!lua_isnumber(L, -1))
	{
		luaL_error(L, "`height' should be a number\n");
	}

	*width = (int)lua_tonumber(L, -2);
	*height = (int)lua_tonumber(L, -1);

	stackDump(L);

	lua_close(L);
}


