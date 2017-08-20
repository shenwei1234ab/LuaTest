
#include <windows.h>
#include <tchar.h>
#include <math.h>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "CELLLuaHelper.h"

#pragma comment(lib,"lua.lib")

#include "OpenGLWindow.h"

class   OpenGL
{
public:
    OpenGL(const char* name)
    {
        int i = 0;
    }
    ~OpenGL()
    {
        int ia = 0;
    }


    void    matrixMode(GLenum mode)
    {
        glMatrixMode(mode);
    }
    void    clear(GLbitfield mask)
    {
        glClear(mask);
    }

    void    loadIdentity()
    {
        glLoadIdentity();
    }

    void    ortho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
    {
        glOrtho(left,right,bottom,top,zNear,zFar);
    }

    void    begin(GLenum mode)
    {
        glBegin(mode);
    }

    void    vertex3f(GLfloat x, GLfloat y, GLfloat z)
    {
        glVertex3f(x,y,z);
    }

    void    end()
    {
        glEnd();
    }

    void    clearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
    {
        glClearColor(red,green,blue,alpha);
    }
};



class   SamplerQTrip :public OpenGLWindow
{
protected:
    lua_State*  L;
public:
    SamplerQTrip()
    {

        L   =   luaL_newstate();
        luaopen_base(L);                    
        luaopen_table(L);                   
        luaopen_io(L);                      
        luaopen_string(L);                  
        luaopen_math(L);
        CELLRegister<OpenGL> reg(L);
        reg.begin();
        reg.exp(&OpenGL::clear,         "clear");
        reg.exp(&OpenGL::matrixMode,    "matrixMode");
        reg.exp(&OpenGL::begin,         "begin");
        reg.exp(&OpenGL::loadIdentity,  "loadIdentity");
        reg.exp(&OpenGL::ortho,         "ortho");
        reg.exp(&OpenGL::vertex3f,      "vertex3f");
        reg.exp(&OpenGL::end,           "endGL");

        reg.exp(&OpenGL::clearColor,    "clearColor");
        reg.expNumber(GL_TRIANGLE_STRIP,"GL_TRIANGLE_STRIP");
        reg.end();
    }
    ~SamplerQTrip()
    {
        lua_close(L);
    }

    struct  Vertex
    {
        float   x,y,z;
    };

    Vertex  _line[360];


    virtual void    render()
    {
#define M_PI (3.14159265358979323846)

        luaL_dofile(L,"main.lua");
//         //! 指定以下的操作针对投影矩阵
//         glMatrixMode(GL_PROJECTION);
//         //! 将投影举证清空成单位矩阵
//         glLoadIdentity();
//         glOrtho(0,_width,_height,0,-100,100);
// 
//         glBegin(GL_TRIANGLE_STRIP);
// 
//         glVertex3f(10,110,0);
//         glVertex3f(10,10,0);
//         glVertex3f(110,110,0);
//         glVertex3f(110,10,0);
// 
//        
//         glEnd();
    }
};

int __stdcall WinMain( 
                      HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, 
                      int nShowCmd
                      )
{

    SamplerQTrip    instance;
    instance.main(800,600);


    return  0;
}