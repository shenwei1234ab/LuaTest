--region *.lua
--Date
--此文件由[BabeLua]插件自动生成


Set = {}
local mt = {}
function Set.new(l)
    local set = {}
    setmetatable(set,mt);
    for _,v in ipairs(l) 
        do set[v] = true
    end
    return set
end


function Set.union(a,b)
    local res = Set.new{}
    for k in pairs(a) do res[k] = true end
    for k in pairs(b) do res[k] = true end
    return res
end


function Set.intersection(a,b)
    local res = Set.new{}
    for k in pairs(a) do 
        res[k] = b[k]
    end
    return res
end

function Set.tostring(set)
    local l = {}
    for e in pairs(set) do 
        l[#l + 1] = e
    end
    return "{" .. table.concat(l,",") .. "}"
end


function Set.print(s)
    print(Set.tostring(s))
end



s1=Set.new{10,20,30,50}
s2=Set.new{30,1}
print(getmetatable(s1))
print(getmetatable(s2))
mt.__add = Set.union
mt.__sub = Set.intersection
mt.__tostring = Set.tostring

s3=s1-s2
--Set.print(s3)
print(s1)
--endregion
