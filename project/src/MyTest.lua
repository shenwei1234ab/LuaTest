--region *.lua
--Date
--此文件由[BabeLua]插件自动生成

--功能：写入文件
--输入：文件名, 内容
--输出：生成的文件里面包含内容
function copyfile(sourcefilePath,fileName,destfilePath)
local sourcefile = io.open(sourcefilePath .. fileName , "r")
local destinationfile = io.open(destfilePath .. fileName, "w")
destinationfile:write(sourcefile:read("*all"))
sourcefile:close()
destinationfile:close()
end

sourcefile = 
{
    "test1.txt",
    "test2.txt",
}
os.execute("cd F:\\lua\\LuaTest");
os.execute("git status");
sourcefilePath = "F:\\lua\\";
destfilePath= "F:\\lua\\test\\";
for i = 1, #sourcefile do  
    --print(sourcefile[i])
    copyfile(sourcefilePath,sourcefile[i],destfilePath);
end  
print("ok................");
a = io.read("*number");
--endregion
