--region *.lua
--Date
--此文件由[BabeLua]插件自动生成





s=[[ a "problematic\\" ]]
s2='a "problematic\\"' 

print(s);
print(string.format("%q",s2));


s = "hello world"
i,j = string.find(s,"hello")
print(i,j)

--endregion
