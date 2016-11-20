#include "LoadFile.hpp"
#include "LoadTable.hpp"

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


int main(void)
{ 
	int a, b;
	//load("F:\\lua\\LuaTest\\project\\src\\config.lua", &a, &b);
	//printf("a=%d,b=%d", a, b);
	loadTable("F:\\lua\\LuaTest\\project\\src\\config.lua");
	getchar();
	return 0;
}