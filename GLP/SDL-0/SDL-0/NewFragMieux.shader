#version 330 core
out vec4 FragColor;
//uniform vec4 ourColor; 
in vec4 ourColor; 

void main()
{
   //FragColor = vec4(1.0f, 0.6f, 0.7f, 1.0f);
   FragColor = ourColor; 
}
