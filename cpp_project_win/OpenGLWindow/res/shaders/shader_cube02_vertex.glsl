#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

//输出到片段着色器
out vec2 v_texCoord;
out vec4 v_color;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(position.x, position.y, position.z, 1.0);
//	gl_Position = u_MVP * position/position.w;
	v_texCoord = texCoord;
	v_color = position;
}
