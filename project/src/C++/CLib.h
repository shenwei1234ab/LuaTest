#include "error.h"

#pragma comment(lib, "lua5.1.lib")

//要导出的dll函数http://www.tuicool.com/articles/YNjAvey
#if defined(_WIN32)
extern "C" _declspec(dllexport)  int luaopen_Func1lib(lua_State* L);

#else
extern "C" int luaopen_Func1lib(lua_State* L);
#endif