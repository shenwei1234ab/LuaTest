#include "LoadFile.h"
#include "LoadTable.h"
#include "LoadFunction.h"
#include "cfunction.h"
#include "LoadLib.h"
void test()
{
	char buff[256];
	int error;
	lua_State *L = lua_open();  /* opens Lua */
	luaopen_base(L);         /* opens the basic library */
	luaopen_table(L);        /* opens the table library */
							 //  luaopen_io(L);           /* opens the I/O library */ //PANIC: unprotected error in call to Lua API (no calling environment)  
	luaL_openlibs(L);       //instead of last line: luaopen_io(L);  
	luaopen_string(L);       /* opens the string lib. */
	luaopen_math(L);         /* opens the math lib. */
	while (fgets(buff, sizeof(buff), stdin) != NULL) {
		error = luaL_loadbuffer(L, buff, strlen(buff), "line") || lua_pcall(L, 0, 0, 0);
		if (error) {
			fprintf(stderr, "%s", lua_tostring(L, -1));
			lua_pop(L, 1);/* pop error message from the stack */
		}
	}
	lua_close(L);
}
//热更新实现
void HotSpain(const char *fileName)
{
	int global_var1 = 0;
	printf("这个程序演示Lua调用C++的Dll：\n");
	lua_State *pLuaState = luaL_newstate(); //也可以用lua_open();
	luaL_openlibs(pLuaState);

	if (luaL_dofile(pLuaState, fileName))// 加载脚本文件
	{
		printf("dofile error.\n");
	}
	//getchar();
	//if (luaL_dofile(pLuaState, fileName))// 第二次加载脚本文件
	//{
	//	printf("dofile error.\n");
	//}
	//getchar();
	lua_close(pLuaState);
}



int main1(void)
{ 
	int a, b;
	const char *fileName = "F:\\lua\\LuaTest\\project\\src\\CLib.lua";
	//load("F:\\lua\\LuaTest\\project\\src\\config.lua", &a, &b);
	//printf("a=%d,b=%d", a, b);
	//loadTable("F:\\lua\\LuaTest\\project\\src\\config.lua");
	//loadFunction("F:\\lua\\LuaTest\\project\\src\\config.lua",1,2);
	HotSpain(fileName);
	getchar();
	HotSpain(fileName);
	getchar();
	return 0;
}