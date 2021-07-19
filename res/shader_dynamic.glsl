#type vertex
#version 330 core
#define DEPTH_MIN .0001
#define DEPTH_MAX .9998
layout(location = 0) in vec2 i_Position;
layout(location = 1) in vec2 i_TexCoord;
layout(location = 2) in float i_TexIndex;
layout(location = 3) in float i_CenterY;

uniform vec2 u_Camera, u_Scale;

out vec2 v_TexCoord, v_Position;
out float v_TexIndex, v_Depth;

void main()
{
	gl_Position = vec4((i_Position + u_Camera) * u_Scale, 0, 1);
	v_TexCoord = i_TexCoord;
	v_TexIndex = i_TexIndex;
	v_Position = i_Position;
	v_Depth = clamp(((i_CenterY + u_Camera.y) * u_Scale.y + 1) / 2, DEPTH_MIN, DEPTH_MAX);
}




#type fragment
#version 330 core
#define MAX_LIGHT_COUNT 100
#define LIGHT_DISTANCE 255
layout(location = 0) out vec4 o_Color;

struct Light
{
	vec4 pos, color;
};
layout(std140) uniform u_Lights
{
	Light light[MAX_LIGHT_COUNT];
} u_Light;
uniform int u_LightCount;

uniform sampler2DArray u_Textures[%MAX_TEXTURES%];
uniform int u_TextureFrames[%MAX_TEXTURES%];
uniform float u_PixelSize;

in vec2 v_TexCoord, v_Position;
in float v_TexIndex, v_Depth;

void main()
{
	vec3 color = vec3(0);
	for(int i = 0; i < u_LightCount; i++)
	{
		// BASIC SMOOTH LIGHTING
		//
		vec3 lightDir = normalize(u_Light.light[i].pos.xyz - vec3(v_Position, 0));
		vec3 normal = vec3(0, 0, 1);
		float intensity = max(0, dot(normal, lightDir));

		// CONCENTRIC RINGS
		// 
		//float x = length(u_Light.light[i].pos.xy - v_Position);
		//float z = int(x / u_PixelSize) * u_PixelSize;
		//float intensity = max(0, (LIGHT_DISTANCE - z) / LIGHT_DISTANCE);

		// PER-SIMULATED PIXEL
		// float x = length(u_Light.light[i].pos.xy - floor(v_Position)) * 2;
		// float z = u_Light.light[i].pos.z * LIGHT_DISTANCE;
		// float intensity = (x > z ? 0 : clamp((z - sqrt(z * z - (x - z) * (x - z))) / LIGHT_DISTANCE, 0, 1));

		color += intensity * u_Light.light[i].color.xyz;
	}
	color = clamp(color, vec3(0), vec3(1));

	int index = int(v_TexIndex);
	o_Color = vec4(color, 1) * texture(u_Textures[index], vec3(v_TexCoord, u_TextureFrames[index]));
	gl_FragDepth = v_Depth;
}