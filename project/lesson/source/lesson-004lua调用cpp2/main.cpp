
#include <stdio.h>

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

    /// ��ջ�ϸ�����������Ԫ����һ������ѹջ��
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
*   luaע�ắ����������������
*/

int add(lua_State* L)
{
    int a = lua_tointeger(L,1);
    int b = lua_tointeger(L,2);

    lua_pushinteger(L,a+b);
    printf("%d",a+b);
    //1 ��ʾѹ��ջ���ݸ���
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


int main(int argc,char** argv)
{
    lua_State*  L   =   luaL_newstate();
    luaopen_base(L);                    
    luaopen_table(L);                   
    luaopen_io(L);                      
    luaopen_string(L);                  
    luaopen_math(L); 

    /// ע��add������lua��ȫ�ֱ���
    lua_register(L,"addEEx",add);

    luaL_dostring(L,"addEEx(111,333)");
    stackDump(L);

    /// 1 ����1 -> luaL_register()
    luaL_openlib(L,"mylib",funcs,0);

     stackDump(L);

    luaL_dostring(L, "mylib.test1(100,4)") ;
    //stackDump(L);

    /// ����2
    luaL_requiref(L,"libs",addTest1,1);
    stackDump(L);
    luaL_dostring(L, "libs.test1(100,1234)") ;


    /// �Ϸ���

    

    //! ��ջ��ȡ�ط���ֵ
    int result = lua_tointeger(L,-1);

    lua_close(L);

    return  0;
}