#ifndef _LOAD_FUNCTION_H
#define _LOAD_FUNCTION_H
#include "error.h"
#include <stdlib.h>
#include <iostream>

/* call a function `f' defined in Lua */

//读取config的lua函数
 void loadFunction(char *filename, double x, double y)
{
	lua_State *L = lua_open();
	luaopen_base(L);
	luaL_openlibs(L);
	luaopen_string(L);
	luaopen_math(L);
	//读取lua文件
	/*if (luaL_loadfile(L, filename))*/
	if(luaL_dofile(L, filename))
	{
		std::cout << "cannot run configuration file" << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);/* pop error message from the stack */
		return;
	}
	double z;
	lua_getglobal(L, "hello");   /* function to be called */
	int ret = 0;
	if ((ret = lua_pcall(L, 0, 0, 0)) != 0)
	{
		std::cout << lua_tostring(L, -1) << std::endl;
		error(L, "error running function `f': %s", lua_tostring(L, -1));
	}

	/* push functions and arguments */
	lua_getglobal(L, "add");   /* function to be called */

	lua_pushnumber(L, x);    /* push 1st argument */

	lua_pushnumber(L, y);    /* push 2nd argument */
							 /* do the call (2 arguments, 1 result) */

	if (lua_pcall(L, 2, 1, 0) != 0)
	{
		error(L, "error running function `f': %s");
	}

	lua_tostring(L, -1);
	/* retrieve result */

	if (!lua_isnumber(L, -1))

		error(L, "function `f' must return a number");

	z = lua_tonumber(L, -1);
	std::cout << "x+y = " << z << std::endl;
	lua_pop(L, 1);  /* pop returned value */
}
#endif

