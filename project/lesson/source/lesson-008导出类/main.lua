print( "test lua access C++ class" )

local function main()
  
    a = Enemy('aa')
    b = Enemy('bb')
	a:setLife(20);
	b:setLife(123);
end


main()
print('finish');