#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in mat4 offset;

uniform mat4 u_model;//使用offset替代
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 v_texCoord;

void main()
{
//	gl_Position = u_projection * u_view  * u_model * vec4(position, 1.0);//使用uniform方式传model
	gl_Position = u_projection * u_view * offset * vec4(position, 1.0);
	v_texCoord = texCoord;
}
