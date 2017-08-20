#include <stdio.h>
#include <iostream>
#include "error.h"
extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}


//c++��ȡLua����
void LoadluaTest()
{
	lua_State *L = lua_open();
	luaopen_base(L);
	luaL_openlibs(L);
	luaopen_string(L);
	luaopen_math(L);

	//��ȡlua�ļ����������
	if (luaL_dofile(L, "F:\\lua\\LuaTest\\project\\src\\loadlua.lua"))
	{
		std::cout << "cannot run configuration file" << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);/* pop error message from the stack */
		return;
	}
	//������ջ
	lua_getglobal(L, "loadlua");   
	stackDump(L);
	//������ջ
	lua_pushnumber(L, 6);    /* push 1st argument */
	lua_pushnumber(L, 7);    /* push 2nd argument */
	
	//���ú�����ͬʱ��Ժ����������������г�ջ��������ѹ�뷵��ֵ
	//2:�������� 1:����ֵ����
	lua_call(L, 2, 1);
	//��ջ��ȡ�ط���ֵ
	int result = lua_tointeger(L, -1);
	//��ջ
	lua_pop(L, 1);
	std::cout << result << std::endl;
}



void main1()
{
	LoadluaTest();
	system("pause");
}