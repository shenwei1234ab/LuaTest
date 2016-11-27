--region *.lua
--Date
--此文件由[BabeLua]插件自动生成

--调用c中的函数

function COS(a)  
print("called COS in lua script")  
--lua调用c/c++  
return DY_MATH.cos(a)  
end  
  
  
function SIN(a)  
print("called SIN in lua script")  
return DY_MATH.sin(a)  
end  
  
  
function SHOWMESSAGE()  
showmessage()  
end  
--endregion  


--endregion
