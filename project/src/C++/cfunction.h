static int math_abs(lua_State *L)
{
	//获取传入的参数  
	lua_pushnumber(L, abs((int)luaL_checknumber(L, 1)));

	return 1;
}

static int math_cos(lua_State *L)
{

	lua_pushnumber(L, cos((double)luaL_checknumber(L, 1)));

	return 1;

}

static int math_sin(lua_State *L)
{

	lua_pushnumber(L, sin((double)luaL_checknumber(L, 1)));


	return 1;
}

static const luaL_reg mathlib[] = {
	{ "abs", math_abs },
	{ "cos", math_cos },
	{ "sin", math_sin },
	{ NULL, NULL }
};


static int ShowMessage(lua_State * L)
{
	lua_pushnumber(L, 1000);
	printf("show message and push 1000 \n");
	return -1;
}



void cfunctionTest(const char *filename)
{
	lua_State* L = lua_open();

	luaL_openlibs(L);

	//  lua_register(L, "cos", math_cos);  
	//  lua_register(L, "sin", math_sin);  
	//  lua_register(L, "abs", math_abs);  


	//#define lua_register(L,n,f) (lua_pushcfunction(L, (f)), lua_setglobal(L, (n)))  
	luaL_register(L, "DY_MATH", mathlib);
	luaL_dofile(L, filename);

	double sinv = 30 * 3.1415926 / 180.0;
	lua_getglobal(L, "SIN");
	lua_pushnumber(L, sinv);
	if (0 != lua_pcall(L, 1, 1, 0))
	{
		printf("cpp call lua function failed\n");
	}
	printf("sin(%f)=%f\n", sinv, lua_tonumber(L, -1));
	lua_pop(L, 1);


	lua_getglobal(L, "COS");
	lua_pushnumber(L, 0.5);
	if (0 != lua_pcall(L, 1, 1, 0))
	{
		printf("cpp call lua function failed\n");
	}
	printf("cos(0.5)=%f\n", lua_tonumber(L, -1));
	lua_pop(L, 1);


	//压栈后设置一个lua可调用的全局函数名  
	lua_pushcfunction(L, ShowMessage);
	lua_setglobal(L, "showmessage");
	//c调用lua  
	lua_getglobal(L, "SHOWMESSAGE");
	lua_pcall(L, 0, 0, 0);
	printf("get the showmessage pushed value %f \n", lua_tonumber(L, -1));

	lua_close(L);

}