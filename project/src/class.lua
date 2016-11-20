--region *.lua
--Date
--此文件由[BabeLua]插件自动生成

Account = 
{
balance = 0,
withdraw = function(self,v)
self.balance = self.balance - v
end
}

function Account:deposit(v)
self.balance = self.balance+v
end

--类
function Account:new(o)
o = o or {}
setmetatable(o,self)
self.__index = self
return o
end

a = Account:new{balance = 0}
a:deposit(100.00)


Account.deposit(Account,200.00)
Account:withdraw(100.00)

--继承
SpecialAccount = Account:new()
s = SpecialAccount:new{limit=1000.00}






Window = {}
Window.prototype = 
{
    x = 0,
    y = 0,
    width = 100,
    height = 100
}
Window.mt = {}

function Window.new(o)
setmetable(o,Window.mt)
return o
end

Window.mt.__index = function(table,key)
return Window.prototype[key]
end

w =Window.new{x=10,y=20}
print(w.width)

--endregion
