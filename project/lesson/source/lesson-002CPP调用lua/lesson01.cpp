
#include <stdio.h>

extern "C"
{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

#pragma comment(lib,"lua.lib")

void    printTable1(lua_State* pState, int index)
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








int main(int argc,char** argv)
{
    lua_State*  L   =   luaL_newstate();

    
    
    luaopen_base(L);   

    luaopen_table(L);   
    
    luaopen_io(L);                      
    luaopen_string(L);                  
    luaopen_math(L);     

    /// 直接执行lua程序代码

    luaL_dostring(L,"print('hello lua')");

    /// 执行lua 文件代码

    luaL_dofile(L,"test.lua");

    /// 查找addFunc函数,并压入栈底
    lua_getglobal(L,"addFunc");

   
    /// 第一个参数压栈
    lua_pushinteger(L,6);
    /// 第二个参数压栈
    lua_pushinteger(L,5);

     

    /// 调用addFunc函数，同时会对addFunc及两个参加进行出栈操作,并压入返回值
    lua_call(L,2,1);
   
    int result = lua_tointeger(L,-1);//从栈中取回返回值

     stackDump(L);
    lua_pop(L,1);//清栈，由于当前只有一个返回值
    printf("result = %d",result);



    lua_close(L);

    return  0;
}