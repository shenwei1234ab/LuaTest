print("Loading CLib.lua")
local testlib = package.loadlib("F://lua//LuaTest//project//prj2015//CPrj//Debug//CLib.dll", "luaopen_Func1lib")
--local testlib = package.loadlib("F://CLib.dll", "luaopen_Func1lib")
print(testlib)--, "Can not open testlib.")
if(testlib)then
	testlib();  --调用DLL中抛出函数
	a,b=Func1lib.GameLogic1(7,6)
	print("average:",a,"sum:",b);
--	a,b=Func1lib.GameLogic2(3,6)
--	print("average:",a,"sum:",b);
else
    print("Error")
end