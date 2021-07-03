#type vertex
#version 330 core
layout(location = 0) in vec2 i_Position;
layout(location = 1) in vec2 i_TexCoord;
layout(location = 2) in float i_TexIndex;

uniform vec2 u_Camera;

out vec2 v_TexCoord;
out float v_TexIndex;

void main()
{
	gl_Position = vec4(i_Position + u_Camera, 0, 1);
	v_TexCoord = i_TexCoord;
	v_TexIndex = i_TexIndex;
}




#type fragment
#version 330 core
layout(location = 0) out vec4 o_Color;

uniform sampler2DArray u_Textures[%MAX_TEXTURES%];
uniform int u_TextureFrames[%MAX_TEXTURES%];

in vec2 v_TexCoord;
in float v_TexIndex;

void main()
{
	int index = int(v_TexIndex);
	o_Color = texture(u_Textures[index], vec3(v_TexCoord, u_TextureFrames[index]));
}