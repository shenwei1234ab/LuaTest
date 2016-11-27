--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
print(package.cpath);
--http://blog.csdn.net/taiq/article/details/6095761
require "mytestlib"  --指定包名称
 
--在调用时，必须是package.function
print(mytestlib.add(1.0,2.0))
print(mytestlib.sub(20.1,19))
--endregion
