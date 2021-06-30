#type vertex
#version 330 core
layout(location = 0) in vec2 i_Position;
layout(location = 1) in vec3 i_Color;

uniform vec2 u_Camera;

out vec3 v_Color;

void main()
{
	gl_Position = vec4(i_Position + u_Camera, 0, 1);
	v_Color = i_Color;
}




#type fragment
#version 330 core
layout(location = 0) out vec4 o_Color;

in vec3 v_Color;

void main()
{
	o_Color = vec4(v_Color, 1);
}