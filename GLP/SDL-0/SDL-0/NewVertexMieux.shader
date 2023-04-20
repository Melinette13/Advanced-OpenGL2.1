#version 330 core
in vec3 pos;
in vec3 color;
out vec4 ourColor;
uniform float YOfest; 
uniform float XOfest;

void main()
{
   
    gl_Position = vec4(pos.x + XOfest, pos.y + YOfest,pos.z, 4.0);
    ourColor = vec4(color, 1.0);

}
