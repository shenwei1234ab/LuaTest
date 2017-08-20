#ifndef register_h
#define register_h

#include <typeinfo>
class Utlis
{
public:

    static  void    printTable(lua_State* pState, int index)
    {
        int top = lua_gettop(pState);

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
            default:
                fprintf(stdout, "%s:%p", lua_typename(pState, type), lua_topointer(pState, -1));
                break;
            }

            fprintf(stdout, "\n");

            lua_pop(pState, 1);
        }

        fprintf(stdout, "}\n");

        lua_settop(pState, top);
    }

    static  void    stackDump( lua_State *l )
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
};



template<typename T>
struct TypeHelper{};

void readValue(TypeHelper<void>, lua_State* vm, int index)
{
}

bool readValue(TypeHelper<bool>, lua_State* vm, int index)
{
    return lua_toboolean(vm, index) == 1;
}

char readValue(TypeHelper<char>, lua_State* vm, int index)
{
    return static_cast<char>(lua_tonumber(vm, index));
}

short readValue(TypeHelper<short>, lua_State* vm, int index)
{
    return static_cast<short>(lua_tonumber(vm, index));
}

int readValue(TypeHelper<int>, lua_State* vm, int index)
{
    return static_cast<int>(lua_tonumber(vm, index));
}

long readValue(TypeHelper<long>, lua_State* vm, int index)
{
    return static_cast<long>(lua_tonumber(vm, index));
}

unsigned char readValue(TypeHelper<unsigned char>, lua_State* vm, int index)
{
    return static_cast<unsigned char>(lua_tonumber(vm, index));
}

unsigned short readValue(TypeHelper<unsigned short>, lua_State* vm, int index)
{
    return static_cast<unsigned short>(lua_tonumber(vm, index));
}

unsigned int readValue(TypeHelper<unsigned int>, lua_State* vm, int index)
{
    return static_cast<unsigned int>(lua_tonumber(vm, index));
}

unsigned long readValue(TypeHelper<unsigned long>, lua_State* vm, int index)
{
    return static_cast<unsigned long>(lua_tonumber(vm, index));
}

char* readValue(TypeHelper<char*>, lua_State* vm, int index)
{
    return const_cast<char*>(lua_tostring(vm, index));
}

const char* readValue(TypeHelper<const char*>, lua_State* vm, int index)
{
    return lua_tostring(vm, index);
}

float readValue(TypeHelper<float>, lua_State* vm, int index)
{
    return static_cast<float>(lua_tonumber(vm, index));
}

double readValue(TypeHelper<double>, lua_State* vm, int index)
{
    return static_cast<double>(lua_tonumber(vm, index));
}

void writeValue(lua_State* vm, bool value)
{
    lua_pushboolean(vm, int(value));
}

void writeValue(lua_State* vm, char value)
{
    lua_pushnumber(vm, value);
}

void writeValue(lua_State* vm, short value)
{
    lua_pushnumber(vm, value);
}

void writeValue(lua_State* vm, int value)
{
    lua_pushnumber(vm, value);
}

void writeValue(lua_State* vm, long value)
{
    lua_pushnumber(vm, value);
}

void writeValue(lua_State* vm, unsigned char value)
{
    lua_pushnumber(vm, value);
}

void writeValue(lua_State* vm, unsigned short value)
{
    lua_pushnumber(vm, value);
}

void writeValue(lua_State* vm, unsigned int value)
{
    lua_pushnumber(vm, value);
}

void writeValue(lua_State* vm, unsigned long value)
{
    lua_pushnumber(vm, value);
}

void writeValue(lua_State* vm, char* value)
{
    lua_pushstring(vm, value);
}

void writeValue(lua_State* vm, const char* value)
{
    lua_pushstring(vm, value);
}

void writeValue(lua_State* vm, float value)
{
    lua_pushnumber(vm, value);
}

void writeValue(lua_State* vm, double value)
{
    lua_pushnumber(vm, value);
}

template<typename RET,typename T>
class LuaClassCaller
{
public:
    static int call(T* obj,RET (T::*func)(), lua_State* pState)
    {
        RET ret =   (obj->*func)();
        writeValue(pState, ret);
        return 1;
    }

    template<typename P1>
    static int call(T* obj,RET (T::*func)(P1), lua_State* pState)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), pState, 2);
        RET ret =   (obj->*func)(p1);
        writeValue(pState, ret);
        return 1;
    }

    template<typename P1, typename P2>
    static int call(T* obj,RET (T::*func)(P1, P2), lua_State* pState)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), pState, 2);
        P2  p2  =   readValue(TypeHelper<P2>(), pState, 3);
        RET ret =   (obj->*func)(p1,p2);
        writeValue(pState, ret);
        return 1;
    }

    template<typename P1, typename P2, typename P3>
    static int call(T* obj,RET (T::*func)(P1, P2, P3), lua_State* pState)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), pState, 2);
        P2  p2  =   readValue(TypeHelper<P2>(), pState, 3);
        P3  p3  =   readValue(TypeHelper<P3>(), pState, 4);
        RET ret =   (obj->*func)(p1,p2,p3);
        writeValue(pState, ret);
        return 1;
    }

    template<typename P1, typename P2, typename P3,typename P4>
    static int call(T* obj,RET (T::*func)(P1, P2, P3,P4), lua_State* pState)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), pState, 2);
        P2  p2  =   readValue(TypeHelper<P2>(), pState, 3);
        P3  p3  =   readValue(TypeHelper<P3>(), pState, 4);
        P4  p4  =   readValue(TypeHelper<P4>(), pState, 5);
        RET ret =   (obj->*func)(p1,p2,p3,p4);
        writeValue(pState, ret);
        return 1;
    }
    template<typename P1, typename P2, typename P3,typename P4,typename P5>
    static int call(T* obj,RET (T::*func)(P1, P2, P3,P4,P5), lua_State* pState)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), pState, 2);
        P2  p2  =   readValue(TypeHelper<P2>(), pState, 3);
        P3  p3  =   readValue(TypeHelper<P3>(), pState, 4);
        P4  p4  =   readValue(TypeHelper<P4>(), pState, 5);
        P5  p5  =   readValue(TypeHelper<P5>(), pState, 6);
        RET ret =   (obj->*func)(p1,p2,p3,p4,p5);
        writeValue(pState, ret);
        return 1;
    }
    template<typename P1, typename P2, typename P3,typename P4,typename P5,typename P6>
    static int call(T* obj,RET (T::*func)(P1, P2, P3,P4,P5,P6), lua_State* pState)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), pState, 2);
        P2  p2  =   readValue(TypeHelper<P2>(), pState, 3);
        P3  p3  =   readValue(TypeHelper<P3>(), pState, 4);
        P4  p4  =   readValue(TypeHelper<P4>(), pState, 5);
        P5  p5  =   readValue(TypeHelper<P5>(), pState, 6);
        P5  p6  =   readValue(TypeHelper<P6>(), pState, 7);
        RET ret =   (obj->*func)(p1,p2,p3,p4,p5,p6);
        writeValue(pState, ret);
        return 1;
    }
};

template<typename T>
class LuaClassCaller<void,T>
{
public:
    static int call(T* obj,void (T::*func)(), lua_State* vm)
    {
        (obj->*func)();
        return 0;
    }

    template<typename P1>
    static int call(T* obj,void (T::*func)(P1), lua_State* vm)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), vm, 2);
        (obj->*func)(p1);
        return  0;
    }

    template<typename P1, typename P2>
    static int call(T* obj,void (T::*func)(P1, P2), lua_State* vm)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), vm, 2);
        P2  p2  =   readValue(TypeHelper<P2>(), vm, 3);
        (obj->*func)(p1, p2);
        return  0;
    }

    template<typename P1, typename P2, typename P3>
    static int call(T* obj,void (T::*func)(P1, P2, P3), lua_State* vm)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), vm, 2);
        P2  p2  =   readValue(TypeHelper<P2>(), vm, 3);
        P3  p3  =   readValue(TypeHelper<P3>(), vm, 4);
        (obj->*func)(p1, p2, p3);
        return  0;
    }


    template<typename P1, typename P2, typename P3,typename P4>
    static int call(T* obj,void (T::*func)(P1, P2, P3,P4), lua_State* pState)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), pState, 2);
        P2  p2  =   readValue(TypeHelper<P2>(), pState, 3);
        P3  p3  =   readValue(TypeHelper<P3>(), pState, 4);
        P4  p4  =   readValue(TypeHelper<P4>(), pState, 5);
        (obj->*func)(p1, p2, p3,p4);
        return 0;
    }
    template<typename P1, typename P2, typename P3,typename P4,typename P5>
    static int call(T* obj,void (T::*func)(P1, P2, P3,P4,P5), lua_State* pState)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), pState, 2);
        P2  p2  =   readValue(TypeHelper<P2>(), pState, 3);
        P3  p3  =   readValue(TypeHelper<P3>(), pState, 4);
        P4  p4  =   readValue(TypeHelper<P4>(), pState, 5);
        P5  p5  =   readValue(TypeHelper<P5>(), pState, 6);
        (obj->*func)(p1, p2, p3,p4,p5);
        return 0;
    }
    template<typename P1, typename P2, typename P3,typename P4,typename P5,typename P6>
    static int call(T* obj,void (T::*func)(P1, P2, P3,P4,P5,P6), lua_State* pState)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), pState, 2);
        P2  p2  =   readValue(TypeHelper<P2>(), pState, 3);
        P3  p3  =   readValue(TypeHelper<P3>(), pState, 4);
        P4  p4  =   readValue(TypeHelper<P4>(), pState, 5);
        P5  p5  =   readValue(TypeHelper<P5>(), pState, 6);
        P5  p6  =   readValue(TypeHelper<P6>(), pState, 7);
        (obj->*func)(p1, p2, p3,p4,p5,p6);
        return 0;
    }
};



template<typename RET>
class LuaFuncCaller
{
public:
    static int call(RET (*func)(), lua_State* vm)
    {
        RET ret = (*func)();
        writeValue(vm, ret);
        return 1;
    }

    template<typename P1>
    static int call(RET (*func)(P1), lua_State* vm)
    {
        P1 p1 = readValue(TypeHelper<P1>(), vm, 1);
        RET ret = (*func)(p1);
        writeValue(vm, ret);
        return 1;
    }

    template<typename P1, typename P2>
    static int call(RET (*func)(P1, P2), lua_State* vm)
    {
        P1 p1 = readValue(TypeHelper<P1>(), vm, 1);
        P2 p2 = readValue(TypeHelper<P2>(), vm, 2);
        RET ret = (*func)(p1, p2);
        writeValue(vm, ret);
        return 1;
    }

    template<typename P1, typename P2, typename P3>
    static int call(RET (*func)(P1, P2, P3), lua_State* vm)
    {
        P1 p1 = readValue(TypeHelper<P1>(), vm, 1);
        P2 p2 = readValue(TypeHelper<P2>(), vm, 2);
        P3 p3 = readValue(TypeHelper<P3>(), vm, 3);
        RET ret = (*func)(p1, p2, p3);
        writeValue(vm, ret);
        return 1;
    }
    template<typename P1, typename P2, typename P3,typename P4>
    static int call(RET (*func)(P1, P2, P3,P4), lua_State* pState)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), pState, 2);
        P2  p2  =   readValue(TypeHelper<P2>(), pState, 3);
        P3  p3  =   readValue(TypeHelper<P3>(), pState, 4);
        P4  p4  =   readValue(TypeHelper<P4>(), pState, 5);
        RET ret =   (*func)(p1, p2, p3,p4);
        writeValue(pState, ret);
        return 1;
    }
    template<typename P1, typename P2, typename P3,typename P4,typename P5>
    static int call(RET (*func)(P1, P2, P3,P4,P5), lua_State* pState)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), pState, 2);
        P2  p2  =   readValue(TypeHelper<P2>(), pState, 3);
        P3  p3  =   readValue(TypeHelper<P3>(), pState, 4);
        P4  p4  =   readValue(TypeHelper<P4>(), pState, 5);
        P5  p5  =   readValue(TypeHelper<P5>(), pState, 6);
        RET ret =   (*func)(p1, p2, p3,p4,p5);
        writeValue(pState, ret);
        return 1;
    }
    template<typename P1, typename P2, typename P3,typename P4,typename P5,typename P6>
    static int call(RET (*func)(P1, P2, P3,P4,P5,P6), lua_State* pState)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), pState, 2);
        P2  p2  =   readValue(TypeHelper<P2>(), pState, 3);
        P3  p3  =   readValue(TypeHelper<P3>(), pState, 4);
        P4  p4  =   readValue(TypeHelper<P4>(), pState, 5);
        P5  p5  =   readValue(TypeHelper<P5>(), pState, 6);
        P5  p6  =   readValue(TypeHelper<P6>(), pState, 7);
        RET ret =   (*func)(p1, p2, p3,p4,p5,p6);
        writeValue(pState, ret);
        return 1;
    }
};



template<>
class LuaFuncCaller<void>
{
public:
    static int call(void (*func)(), lua_State* vm)
    {
        (*func)();
        return 0;
    } 

    template<typename P1>
    static int call(void (*func)(P1), lua_State* vm)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), vm, 1);
        (*func)(p1);
        return 0;
    }

    template<typename P1, typename P2>
    static int call(void (*func)(P1, P2), lua_State* vm)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), vm, 1);
        P2  p2  =   readValue(TypeHelper<P2>(), vm, 2);
        (*func)(p1, p2);
        return 0;        
    }

    template<typename P1, typename P2, typename P3>
    static int call(void (*func)(P1, P2, P3), lua_State* vm)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), vm, 1);
        P2  p2  =   readValue(TypeHelper<P2>(), vm, 2);
        P3  p3  =   readValue(TypeHelper<P3>(), vm, 3);
        (*func)(p1, p2, p3);
        return 0;        
    }

    template<typename P1, typename P2, typename P3,typename P4>
        static int call(void (*func)(P1, P2, P3,P4), lua_State* pState)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), pState, 2);
        P2  p2  =   readValue(TypeHelper<P2>(), pState, 3);
        P3  p3  =   readValue(TypeHelper<P3>(), pState, 4);
        P4  p4  =   readValue(TypeHelper<P4>(), pState, 5);
        (*func)(p1, p2, p3,p4);
        return 0;
    }
    template<typename P1, typename P2, typename P3,typename P4,typename P5>
    static int call(void (*func)(P1, P2, P3,P4,P5), lua_State* pState)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), pState, 2);
        P2  p2  =   readValue(TypeHelper<P2>(), pState, 3);
        P3  p3  =   readValue(TypeHelper<P3>(), pState, 4);
        P4  p4  =   readValue(TypeHelper<P4>(), pState, 5);
        P5  p5  =   readValue(TypeHelper<P5>(), pState, 6);
        (*func)(p1, p2, p3,p4,p5);
        return 0;
    }
    template<typename P1, typename P2, typename P3,typename P4,typename P5,typename P6>
    static int call(void (*func)(P1, P2, P3,P4,P5,P6), lua_State* pState)
    {
        P1  p1  =   readValue(TypeHelper<P1>(), pState, 2);
        P2  p2  =   readValue(TypeHelper<P2>(), pState, 3);
        P3  p3  =   readValue(TypeHelper<P3>(), pState, 4);
        P4  p4  =   readValue(TypeHelper<P4>(), pState, 5);
        P5  p5  =   readValue(TypeHelper<P5>(), pState, 6);
        P5  p6  =   readValue(TypeHelper<P6>(), pState, 7);
        (*func)(p1, p2, p3,p4,p5,p6);
        return 0;
    }
};




template<typename Func,typename T>
class ClassCallDispatcher
{
public:
    template<typename RET>
    static int dispatch(T*obj,RET (T::*func)(), lua_State* vm)
    {
        return LuaClassCaller<RET,T>::call(obj,func, vm);
    }

    template<typename RET, typename P1>
    static int dispatch(T*obj,RET (T::*func)(P1), lua_State* vm)
    {
        return LuaClassCaller<RET,T>::call(obj,func, vm);
    }

    template<typename RET, typename P1, typename P2>
    static int dispatch(T*obj,RET (T::*func)(P1, P2), lua_State* vm)
    {
        return LuaClassCaller<RET,T>::call(obj,func, vm);
    }

    template<typename RET, typename P1, typename P2, typename P3>
    static int dispatch(T*obj,RET (T::*func)(P1, P2, P3), lua_State* vm)
    {
        return LuaClassCaller<RET,T>::call(obj,func, vm);
    }
};



template<typename Func>
class FuncCallDispatcher
{
public:
    template<typename RET>
    static int dispatch(RET (*func)(), lua_State* vm)
    {
        return LuaFuncCaller<RET>::call(func, vm);
    }

    template<typename RET, typename P1>
    static int dispatch(RET (*func)(P1), lua_State* vm)
    {
        return LuaFuncCaller<RET>::call(func, vm);
    }

    template<typename RET, typename P1, typename P2>
    static int dispatch(RET (*func)(P1, P2), lua_State* vm)
    {
        return LuaFuncCaller<RET>::call(func, vm);
    }

    template<typename RET, typename P1, typename P2, typename P3>
    static int dispatch(RET (*func)(P1, P2, P3), lua_State* vm)
    {
        return LuaFuncCaller<RET>::call(func, vm);
    }
};

void*   getRealFunc(lua_State* vm)
{
    return lua_touserdata(vm, lua_upvalueindex(1));
}
template<typename T>
T*   getObject(lua_State* vm,const char* className)
{
    luaL_checktype( vm, 1, LUA_TUSERDATA );
    void*   userData    =   luaL_checkudata( vm, 1, className );
    return *(T**)userData;
}

template<typename Func>
class FuncRegister
{
public:
    static int call(lua_State* vm)
    {
        Func* func = static_cast<Func*>(getRealFunc(vm));
        return FuncCallDispatcher<Func>::dispatch(*func, vm);
    }
};


template<typename Func>
void regFunction(lua_State* vm, Func func, const char* funcName)
{
    int     size    =   sizeof(Func);
    void*   data    =   lua_newuserdata(vm, size);
    memcpy( data, &func, size);

    lua_pushcclosure(vm, FuncRegister<Func>::call, 1);
    lua_setglobal(vm, funcName);
}

template<typename T>
const char* getClassName()
{
    //! pName = "class xxxx"
    //! 需要的是xxxx
    const char* pName    =   typeid(T).name();
    
    return  pName + 6;
}

template<typename Func,typename T>
class ClassRegister
{

public:
    static int call(lua_State* vm)
    {
        /**
        *   获取真是的函数地址
        */
        Func*   func    =   static_cast<Func*>(getRealFunc(vm));
        /**
        *   获取对象地址
        */
        const char* cName   =   getClassName<T>();
        T*          obj     =   getObject<T>( vm, cName);
        return ClassCallDispatcher<Func,T>::dispatch(obj,*func, vm);
    }
};


template<typename CLASS>
class   CELLRegister
{
public:
    lua_State*  _vm;
public:
    CELLRegister(lua_State* vm)
    {
        _vm =   vm;
    }
    static  int     create(lua_State* vm)
    {
        Utlis::stackDump(vm);
        const char*name = lua_tostring(vm,-1);
        CLASS*  obj =   new CLASS(name);
        void**  p   =   (void**)lua_newuserdata( vm, sizeof(void*));  
                *p  =   obj;
        const char* cName   =   getClassName<CLASS>();
        luaL_getmetatable( vm, cName);
        lua_setmetatable( vm, -2 );
        Utlis::stackDump(vm);
        return  1;
    }
    static  int     destroy(lua_State* vm)
    {
        CLASS*  obj =   (CLASS*)(*(void**)lua_touserdata(vm,-1));
        delete  obj;
        return  0;
    }
    /**
    *   开始注册
    */
    void    begin()
    {
        //1. new method table for l to save functions
        lua_newtable(_vm);
        int methodTable = lua_gettop(_vm);
        //2.new metatable for L to save "__metatable", "__index",  "__gc", etc
        Utlis::stackDump(_vm);
        const char* cName   =   getClassName<CLASS>();
        luaL_newmetatable(_vm, cName);
        int metaTable = lua_gettop(_vm);
        Utlis::stackDump(_vm);
        //3.0 metatable["__metatable"] = methodtable;
        lua_pushliteral( _vm, "__metatable" );
        lua_pushvalue( _vm, methodTable );
        lua_rawset( _vm, metaTable );

        Utlis::stackDump(_vm);

        //4.0 metatable["__index"] = methodtable
        lua_pushliteral( _vm, "__index" );
        lua_pushvalue( _vm, methodTable );
        Utlis::stackDump(_vm);
        lua_rawset( _vm, metaTable );  // the same as lua_settable(1,metatable)
        Utlis::stackDump(_vm);

        //5.0 metatable["__gc"] = gc_animal  //will be called when lua_close(l)
        lua_pushliteral( _vm, "__gc" ); 
        lua_pushcfunction( _vm, CELLRegister<CLASS>::destroy);
        lua_rawset( _vm, metaTable );
        Utlis::stackDump(_vm);
        lua_pop(_vm,1);	
    }

    /**
    *   注册
    */
    template<typename Func>
    void    exp(Func func,const char* funName)
    {
        int     size    =   sizeof(Func);
        void*   data    =   lua_newuserdata(_vm, size);
        memcpy( data, &func, size);

        lua_pushcclosure(_vm, ClassRegister<Func,CLASS>::call, 1);

        lua_setfield(_vm, -2, funName);
        Utlis::stackDump(_vm);
    }

    /**
    *   完成注册
    */
    void    end()
    {
        const char* cName   =   getClassName<CLASS>();
        lua_pop(_vm,1);
        lua_register(_vm, cName , CELLRegister<CLASS>::create );
    }
};
#endif