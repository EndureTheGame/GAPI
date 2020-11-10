#Shader Vertex
#version 330 core
layout(location = 0)in vec4 position;
layout(location = 1)in vec4 aColor;

out vec4 vertexColor;

void main()                
{
  gl_Position = position; 
  vertexColor = aColor;
};                         

#Shader Fragment
#version 330 core
out vec4 color;  
in vec4 vertexColor;

//uniform vec4 u_Color;


void main()                
{              
	color = vertexColor; //vec4(0.5, 0, 1.0, 1.0);
		//vertexColor;
};							