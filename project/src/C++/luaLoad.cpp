#include <stdio.h>
#include <iostream>
#include "error.h"
extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

int add(lua_State* L)
{
	int a = lua_tointeger(L, 1);
	int b = lua_tointeger(L, 2);

	lua_pushinteger(L, a + b);
	printf("%d", a + b);
	//1 表示压入栈数据个数
	return 1;
}

luaL_Reg    funcs[] =
{
	{ "test1",add },
	{ "test2",add },
	{ "test3",add },
	{ "test4",add },
	{ "test5",add },
	{ "test6",add },
	{ "test7",add },
	{ 0,0 },
};

int addTest1(lua_State* L)
{
	luaL_newlib(L, funcs);
	return 1;
}
//Lua调用c++函数
void luaLoadFun()
{
	lua_State*  L = luaL_newstate();
	luaopen_base(L);
	luaopen_table(L);
	luaopen_string(L);
	luaopen_math(L);
	///方法1注册add函数到lua的全局表中,建立函数名称和函数地址的映射
	lua_register(L, "add", add);
	//lua调用
	//luaL_dostring(L, "print(add(111,333))");
	///方法2:注册多个函数到lua的全局表中
	luaL_openlib(L, "mylib", funcs, 0);
	//luaL_dostring(L, "mylib.test1(100,4)");

	/// 建议方法2
	luaL_requiref(L, "libs", addTest1, 1);
	luaL_dostring(L, "libs.test1(100,1234)");
}

void main()
{
	luaLoadFun();
	system("pause");
}