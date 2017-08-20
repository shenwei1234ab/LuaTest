
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#pragma comment(lib,"lua.lib")




void    printTable(lua_State* pState, int index)
{
    int top = lua_gettop(pState);

    /// 把栈上给定索引处的元素作一个副本压栈。
    lua_pushvalue(pState, index);

    fprintf(stdout, "{\n");

    lua_pushnil(pState);

    while(lua_next(pState, -2))
    {
        fprintf(stdout, "\t");
        int type = lua_type(pState, -2);
        switch(type)
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
        switch(type)
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

void    stackDump( lua_State *l )
{
    int i;
    int top = lua_gettop( l );
    printf("------start-----%d\n", top);

    for( i = 1; i <= top; i++ )
    {
        int t = lua_type( l, i );
        printf("type: %s value:", lua_typename(l,t));

        switch (t)
        {
        case LUA_TSTRING:
            printf("%s", lua_tostring(l,i));
            break;
        case LUA_TBOOLEAN:
            printf( lua_toboolean(l,i)? "true" : "false" );
            break;
        case LUA_TNUMBER:
            printf("%g", lua_tonumber(l,i));
            break;
        case LUA_TTABLE:
            printTable(l, i);
            break;
        default:
            printf("%s", lua_typename(l,t));
            break;
        }
        printf("\n");
    }

    printf("------end----\n" );
}







/**
*   lua注册函数与宿主函数分离
*/

int add(lua_State* L)
{
    int a = lua_tointeger(L,1);
    int b = lua_tointeger(L,2);

    lua_pushinteger(L,a+b);
    printf("%d",a+b);
    //1 表示压入栈数据个数
    return 1;
}




luaL_Reg    funcs[] =   
{
    {"test1",add},
    {"test2",add},
    {"test3",add},
    {"test4",add},
    {"test5",add},
    {"test6",add},
    {"test7",add},
    {0,0},
};


int addTest1(lua_State* L)
{
    luaL_newlib(L,funcs);
    return 1;
}


int gcFunction(lua_State* L)
{
    return 1;
}

int main(int argc,char** argv)
{
    lua_State*  L   =   luaL_newstate();
    luaopen_base(L);                    
    luaopen_table(L);                   
    luaopen_io(L);                      
    luaopen_string(L);                  
    luaopen_math(L); 

    //1. new method table 保存所有的函数
    lua_newtable(L);

    stackDump(L);

    int methodTable = lua_gettop(L);
    //2. new metatable for L to save "__metatable", "__index",  "__gc", etc
    luaL_newmetatable(L, "myName" );
    stackDump(L);

    int metaTable = lua_gettop(L);

    stackDump(L);
    //3. metatable["__metatable"] = methodtable;同 lua_settable(1,metatable)
    lua_pushliteral( L, "__metatable" );
    lua_pushvalue( L, methodTable );
    stackDump(L);
    lua_rawset( L, metaTable );

    stackDump(L);


    //4. metatable["__index"] = methodtable ;同 lua_settable(1,metatable)
    lua_pushliteral( L, "__index" );
    lua_pushvalue( L, methodTable );

    stackDump(L);
    lua_rawset( L, metaTable );  

    stackDump(L);
   
    //5. metatable["__gc"] = myFunction  
    lua_pushliteral( L, "__gc" ); 
    lua_pushcfunction( L, gcFunction );

    stackDump(L);
    lua_rawset( L, metaTable );
    stackDump(L);
    lua_close(L);

    return  0;
}