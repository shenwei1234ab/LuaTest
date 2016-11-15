--region *.lua
--Date
--此文件由[BabeLua]插件自动生成



local count = 0;
function Data(data)
    count = count +1
end
--读取Data文件中的值
dofile("F:\\lua\\project\\src\\Data.lua")
print("number is "..count)


--串行化
function serialize(o)
    if type(o) == "number" then
        io.write(o)
    elseif type(o) == "string" then
--        io.write("[[",o,"]]")
        io.write(string.format("%q",o))
    elseif type(o) == "table" then
        io.write("{\n")
    for k,v in pairs(o) do
        io.write(" ",k,"=")
        serialize(v)
        io.write(",\n")
    end
    io.write("}\n")
    else
        error("cannot serialize a" .. type(o))
    end
        end
end



serialize{a=12,b='Lua',key='author "one"'}






--endregion
