
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <typeinfo>
#include <vector>

#include <windows.h>
#include <gl/GL.h>
#pragma comment(lib,"OpenGL32.lib")

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "CELLLuaHelper.h"

#pragma comment(lib,"lua.lib")


class   OpenGL
{
public:
    OpenGL(const char* name)
    {
        int i = 0;
    }

    void    clear(GLbitfield mask)
    {
        glClear(mask);
    }

    void    clearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
    {
        glClearColor(red,green,blue,alpha);
    }
};

int __cdecl main(int argc,char** argv)
{
    lua_State*  L   =   luaL_newstate();
    luaopen_base(L);                    
    luaopen_table(L);                   
    luaopen_io(L);                      
    luaopen_string(L);                  
    luaopen_math(L);


    CELLRegister<OpenGL> reg(L);
    reg.begin();
    reg.exp(&OpenGL::clear,"clear");
    reg.exp(&OpenGL::clearColor,"clearColor");
    reg.end();

    luaL_dofile( L, "main.lua" );
    Utlis::stackDump(L);
    lua_close(L);

    return  0;
}