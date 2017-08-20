
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

#include "CELLLuaHelper.h"

#pragma comment(lib,"lua.lib")


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


void    test(int a,char* data,double pp)
{

}

const char*    test11(int a,char* data,double pp)
{
    return  "ete";
}

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

    regFunction(L,test,"test");
    regFunction(L,test11,"test11");

   
    if( luaL_dofile( L, "main.lua" ))
    {  
    }
    lua_close(L);

    return  0;
}