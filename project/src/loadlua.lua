print("fact function Test");

function fact(n)
	if n== 0 then
		return 1
	else
		return n*fact(n-1)
	end
end

--print("enter a number");
--a = io.read("*number");
--print(fact(a));


function test1(n)
	n = 2;
end

function test2()
    n = 3;
end

n = 1;
test1(n);
print(n)



function loadlua(x,y)
    print("HelloLua");
    return x+y;
end

