--region *.lua
--Date
--此文件由[BabeLua]插件自动生成

--创建迭代器
function values(t)
    local i=0;
    return function() 
                i=i+1
                return t[i]
                end
end 


t = {10,20,30}
iter = values(t)
while true do
    local element = iter()
    if element == nil 
        then break
    end
    print(element)
end


t = {10,20,30}
for element in values(t)do
print(element)
end



--endregion
