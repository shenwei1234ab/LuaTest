#include "CLib.h"

class Father
{
public:
	virtual void test() = 0;
};

class S1 :public Father
{
public:
	virtual void test()
	{
		std::cout << "s1" << std::endl;
	}
};
class S2 :public Father
{
public:
	virtual void test()
	{
		std::cout << "s2" << std::endl;
	}
};
Father *pFather = new S2();
int GameLogic1(lua_State *L)
{
	pFather->test();
	int n = lua_gettop(L);
	double sum = 0;
	int i;

	for (i = 1; i <= n; i++)
	{

		sum += lua_tonumber(L, i);
	}

	lua_pushnumber(L, sum / n);

	lua_pushnumber(L, sum);

	return 2;
}


const struct luaL_Reg Func1lib[] = {
	{ "GameLogic1", GameLogic1 },
	{ NULL, NULL }
};


int luaopen_Func1lib(lua_State* L)
{
	luaL_openlib(L, "Func1lib", Func1lib, 0);
	return 1;
}