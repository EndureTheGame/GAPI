#Shader Vertex
#version 330 core

layout(location = 0)in vec4 position;
layout(location = 1)in vec4 aColor;
layout(location = 2)in vec2 aTexCor;

out vec4 vertexColor;
out vec2 texCor;

void main()                
{
  gl_Position = position; 
  vertexColor = aColor;
  texCor = aTexCor;
};                         

#Shader Fragment
#version 330 core

in vec4 vertexColor;
in vec2 texCor;

out vec4 tex;  

uniform vec4 u_Color;
uniform sampler2D u_Texture;


void main()                
{              
	tex = texture(u_Texture, texCor) * vertexColor * u_Color;;
};							