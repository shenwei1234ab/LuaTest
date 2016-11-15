--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
--http://blog.sina.com.cn/s/blog_547c04090100qfps.html

--闭包:函数对象
function newCounter()
    local i=0
        return function()          --重载()
                i=i+1
                return i
                end
    end


c1 = newCounter()
print(c1())
print(c1())




--endregion
