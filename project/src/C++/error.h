#pragma once
#include "lua.hpp" 

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
static void error(lua_State *L, const char *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
	//std::cout << fmt << std::endl;
}


static void    printTable(lua_State* pState, int index)
{
	int top = lua_gettop(pState);

	/// 把栈上给定索引处的元素作一个副本压栈。
	lua_pushvalue(pState, index);

	fprintf(stdout, "{\n");

	lua_pushnil(pState);

	while (lua_next(pState, -2))
	{
		fprintf(stdout, "\t");
		int type = lua_type(pState, -2);
		switch (type)
		{
		case LUA_TNUMBER:
			fprintf(stdout, "%g", lua_tonumber(pState, -2));
			break;
		case LUA_TBOOLEAN:
			fprintf(stdout, "%d", int(lua_toboolean(pState, -2)));
			break;
		case LUA_TSTRING:
			fprintf(stdout, "%s", lua_tostring(pState, -2));
			break;
		default:
			fprintf(stdout, "%s:%p", lua_typename(pState, type), lua_topointer(pState, -2));
			break;
		}

		fprintf(stdout, "\t\t=\t");

		type = lua_type(pState, -1);
		switch (type)
		{
		case LUA_TNUMBER:
			fprintf(stdout, "%g", lua_tonumber(pState, -1));
			break;
		case LUA_TBOOLEAN:
			fprintf(stdout, "%d", int(lua_toboolean(pState, -1)));
			break;
		case LUA_TSTRING:
			fprintf(stdout, "%s", lua_tostring(pState, -1));
			break;
		case  LUA_TFUNCTION:
			fprintf(stdout, "%s:%p", lua_typename(pState, type), lua_topointer(pState, -1));
			break;
		case LUA_TUSERDATA:
			fprintf(stdout, "%s:%s", lua_typename(pState, type), lua_tostring(pState, -1));
			break;
		default:
			fprintf(stdout, "%s:%p", lua_typename(pState, type), lua_topointer(pState, -1));
		}

		fprintf(stdout, "\n");

		lua_pop(pState, 1);
	}

	fprintf(stdout, "}\n");

	lua_settop(pState, top);
}


//遍历整个栈
//栈底开始算 1 , 2 ,3
//栈顶开始算 -1  -2  -3
static void stackDump(lua_State *l)
{
	int i;
	int top = lua_gettop(l);
	printf("------start-----%d\n", top);

	for (i = 1; i <= top; i++)
	{
		int t = lua_type(l, i);
		printf("type: %s value:", lua_typename(l, t));

		switch (t)
		{
		case LUA_TSTRING:
			printf("%s", lua_tostring(l, i));
			break;
		case LUA_TBOOLEAN:
			printf(lua_toboolean(l, i) ? "true" : "false");
			break;
		case LUA_TNUMBER:
			printf("%g", lua_tonumber(l, i));
			break;
		case LUA_TTABLE:
			printTable(l, i);
			break;
		default:
			printf("%s", lua_typename(l, t));
			break;
		}
		printf("\n");
	}

	printf("------end----\n");
}