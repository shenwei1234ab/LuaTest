#pragma once
#include "lua.hpp" 
#include <stdlib.h>
#include <iostream>
#include "error.h"
#define MAX_COLOR 255
struct ColorTable {

	char *name;

	unsigned char red, green, blue;

} colortable[] = {

	{ "WHITE",  MAX_COLOR, MAX_COLOR, MAX_COLOR },

	{ "RED",    MAX_COLOR, 0,     0 },

	{ "GREEN",  0,    MAX_COLOR, 0 },

	{ "BLUE",   0,    0,         MAX_COLOR },

	{ "BLACK",  0,  0,         0 },
{NULL,     0,         0,         0}     /* sentinel */

};


void setfield(lua_State *L ,const char *index, int value)
{
	lua_pushstring(L, index);
	lua_pushnumber(L, (double)value / MAX_COLOR);
	//lua_settable��table��ջ�е�������Ϊ����������ջ�е�key��value��ջ����������ֵ�޸�table
	lua_settable(L, -3);
}

void setcolor(lua_State *L, ColorTable *ct)
{
	lua_newtable(L);            /* creates a table */

	setfield(L,"r", ct->red);     /* table.r = ct->r */

	setfield(L,"g", ct->green);   /* table.g = ct->g */

	setfield(L,"b", ct->blue);    /* table.b = ct->b */

	lua_setglobal(L,ct->name);    /* 'name' = table */
}



//ջ��->ջ��  -1 -2 -3 -4
//ջ��->ջ�� 1,2,3,4,5
//#define MAX_COLOR 255
int getfield(lua_State *L, const char *key)
{
	int result;
	//lua_pushstring(L, key);
	//lua_gettable(L, -2);		//��ջ�е���key,ѹ��value
	lua_getfield(L, -1, key);
	if (!lua_isnumber(L, -1))
	{
		error(L, "invalid componet in background color");
	}
	result = (int)lua_tonumber(L, -1);

	stackDump(L);
	lua_pop(L, 1);
	return result;
}





//��ȡconfig.lua�е�ȫ��table
 void loadTable(char *filename)
{
	lua_State *L = lua_open();
	luaopen_base(L);
	luaL_openlibs(L);
	luaopen_string(L);
	luaopen_math(L);
	//��ȡlua�ļ���ѹ��luaջ
	if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
	{
		std::cout << "cannot run configuration file" << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);/* pop error message from the stack */
		return;
	}
	lua_getglobal(L, "background");
	if (!lua_istable(L, -1))
	{
		error(L, " 'backgound' is not a table");
	}
	int red = getfield(L, "r");
	int g = getfield(L, "g");
	int b = getfield(L, "b");
	std::cout << "red:" << red << std::endl;
	std::cout << "g:" << g << std::endl;
	std::cout << "b:" << b << std::endl;
}




