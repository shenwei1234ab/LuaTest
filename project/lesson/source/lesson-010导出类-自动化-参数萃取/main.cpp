
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <typeinfo>

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

class   Enemy
{
protected:
    char    _name[32];
    int     _life;
public:
    Enemy( const char *name )
    {
        strcpy(_name,name);
        _life   =   0;
    }
    ~Enemy(void)
    {}
    void    setLife( int life)
    {
        _life   =   life;
    }
    int     getLife()
    {
        return  _life;
    }
};


class   LuaEnemy
{
public:
    LuaEnemy()
    {

    }
    ~LuaEnemy(void)
    {

    }
    static void Register( lua_State *L )
    {
        stackDump(L);
        //1. new method table 保存所有的函数
        lua_newtable(L);
    
        stackDump(L);
        int methodTable = lua_gettop(L);
        //2. new metatable for L to save "__metatable", "__index",  "__gc", etc
        luaL_newmetatable(L, "Enemy" );

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
        lua_rawset( L, metaTable );  
        stackDump(L);

        //5. metatable["__gc"] = myFunction  
        lua_pushliteral( L, "__gc" ); 
        lua_pushcfunction( L, gc_Enemy );
        lua_rawset( L, metaTable );

        stackDump(L);

        lua_pop(L,1);


        stackDump(L);
        lua_pushcclosure(L, LuaEnemy::setLife,0);
        lua_setfield(L, -2, "setLife");

        stackDump(L);
        lua_pop(L,1);
        

        lua_register(L, "Enemy", LuaEnemy::create );
    }

    static int create( lua_State *L )
    {
        stackDump(L);
        char*   name=   (char*)lua_tostring(L,-1);
        Enemy*  a   =   new Enemy(name);
        void**  p   =   (void**)lua_newuserdata( L, sizeof(void*));  
                *p  =   a;

        luaL_getmetatable( L, "Enemy" );
        stackDump(L);
        lua_setmetatable( L, -2 );
        stackDump(L);
        return 1;
    }
    static int gc_Enemy( lua_State *L )
    {
        stackDump(L);
        Enemy*  enemy   = (Enemy*)(*(void**)lua_touserdata(L,-1));
        stackDump(L);
        delete  enemy;
        return  0;
    }

    static int setLife(lua_State *L)
    {
        stackDump(L);
        int     life    =   lua_tonumber(L,2);
        luaL_checktype( L, 1, LUA_TUSERDATA );
        void*   ud      =   luaL_checkudata( L, 1, "Enemy" );
        Enemy*  enemy   =   *(Enemy**)ud;
        enemy->setLife(life);
        return  0;   
    }
    static int getLife(lua_State *L)
    {
        return  1;
    }
};


template<typename T>
const char* getClassName()
{
    //! pName = "class xxxx"
    //! 需要的是xxxx
    const char* pName    =   typeid(T).name();
    return  pName + 6;
}


template<typename T>
T*   getObject(lua_State* vm,const char* className)
{
    luaL_checktype( vm, 1, LUA_TUSERDATA );
    void*   userData    =   luaL_checkudata( vm, 1, className );
    return *(T**)userData;
}

/**
*   该类将参数信息萃取出来了
*/
template<typename Func,typename T>
class ClassCallDispatcher
{
public:
    template<typename RET>
    static int dispatch(T*obj,RET (T::*func)(), lua_State* vm)
    {
        return  0;
    }

    template<typename RET, typename P1>
    static int dispatch(T*obj,RET (T::*func)(P1), lua_State* vm)
    {
        return  0;
    }

    template<typename RET, typename P1, typename P2>
    static int dispatch(T*obj,RET (T::*func)(P1, P2), lua_State* vm)
    {
        return  0;
    }

    template<typename RET, typename P1, typename P2, typename P3>
    static int dispatch(T*obj,RET (T::*func)(P1, P2, P3), lua_State* vm)
    {
        return  0;
    }
    template<typename RET, typename P1, typename P2, typename P3,typename P4>
    static int dispatch(T*obj,RET (T::*func)(P1, P2, P3,P4), lua_State* vm)
    {
        return  0;
    }

    template<typename RET, typename P1, typename P2, typename P3,typename P4,typename P5>
    static int dispatch(T*obj,RET (T::*func)(P1, P2, P3,P4,P5), lua_State* vm)
    {
        return  0;
    }
};




template<typename Function,typename CLASS>
class ClassLuaCall
{

public:
    static int call(lua_State* vm)
    {
        /**
        *   获取真是的函数地址
        */
        Function*   func    =   static_cast<Function*>(lua_touserdata(vm, lua_upvalueindex(1)));
        /**
        *   获取对象地址
        */
        const char* cName   =   getClassName<CLASS>();
        CLASS*      obj     =   getObject<CLASS>( vm, cName);
        ///! 下面该怎么做呢
        ///! 准备根据函数的参数类型，提取函数的参数和返回值
        return ClassCallDispatcher<Function,CLASS>::dispatch(obj,*func, vm);
    }
};

template<typename CLASS>
class   CELLRegister
{
protected:
    lua_State*  _vm;
public:
    CELLRegister(lua_State*vm)
        :_vm(vm)
    {}
    ~CELLRegister()
    {}

    
    /**
    *   开始导出函数
    */
    CELLRegister&   begin()
    {
        //1. new method table 保存所有的函数
        lua_newtable(_vm);

        int methodTable = lua_gettop(_vm);
        //2. new metatable for L to save "__metatable", "__index",  "__gc", etc
        luaL_newmetatable(_vm, getClassName<CLASS>() );

        int metaTable = lua_gettop(_vm);

        //3. metatable["__metatable"] = methodtable;同 lua_settable(1,metatable)
        lua_pushliteral( _vm, "__metatable" );
        lua_pushvalue( _vm, methodTable );
        lua_rawset( _vm, metaTable );
        
        //4. metatable["__index"] = methodtable ;同 lua_settable(1,metatable)
        lua_pushliteral( _vm, "__index" );
        lua_pushvalue( _vm, methodTable );
        lua_rawset( _vm, metaTable );  
       
        //5. metatable["__gc"] = myFunction  
        lua_pushliteral(_vm, "__gc" ); 
        lua_pushcfunction(_vm, CELLRegister<CLASS>::destryObject);
        lua_rawset( _vm, metaTable );

        lua_pop(_vm,1);

        return  *this;
    }
    /**
    *   导出函数 
    */
    template<typename Function>
    CELLRegister&   exp(Function func,const char* name)
    {
        int     size    =   sizeof(func);
        void*   data    =   lua_newuserdata(_vm, size);
        memcpy( data, &func, size);

        lua_pushcclosure(_vm, ClassLuaCall<Function,CLASS>::call, 1);

        lua_setfield(_vm, -2, name);
        return  *this;
    }
    /**
    *   完成导出
    */
    void            end()
    {
        const char* cName   =   getClassName<CLASS>();
        lua_pop(_vm,1);
        lua_register(_vm, cName , CELLRegister<CLASS>::createObject);
    }

    static int createObject( lua_State *L )
    {
        stackDump(L);
        char*   name=   (char*)lua_tostring(L,-1);
        CLASS*  a   =   new CLASS(name);
        void**  p   =   (void**)lua_newuserdata( L, sizeof(void*));  
                *p  =   a;
        luaL_getmetatable( L, "Enemy" );
        lua_setmetatable( L, -2 );
        return 1;
    }
    static int destryObject( lua_State *L )
    {
        CLASS*  obj   = (CLASS*)(*(void**)lua_touserdata(L,-1));
        stackDump(L);
        delete  obj;
        return  0;
    }

};
int main(int argc,char** argv)
{
    lua_State*  L   =   luaL_newstate();
    luaopen_base(L);                    
    luaopen_table(L);                   
    luaopen_io(L);                      
    luaopen_string(L);                  
    luaopen_math(L); 



    CELLRegister<Enemy> reg(L);
    reg.begin();
    reg.exp(&Enemy::getLife,"getLife");
    reg.exp(&Enemy::setLife,"setLife");
    reg.end();

   
    if( luaL_dofile( L, "main.lua" ))
    {  
        stackDump(L);
    }
    lua_close(L);

    return  0;
}