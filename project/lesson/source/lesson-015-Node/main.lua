
local function main()
  
    local gl = OpenGL('aa');
    
	gl:loadIdentity();
    gl:ortho(0,800,600,0,-100,100);

    gl:begin(gl.GL_TRIANGLE_STRIP);

    gl:vertex3f(10,110,0);
    gl:vertex3f(10,10,0);
    gl:vertex3f(110,110,0);
    gl:vertex3f(110,10,0);
	gl:endGL();
end


main()