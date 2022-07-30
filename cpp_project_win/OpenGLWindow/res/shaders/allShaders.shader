#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

//输出到片段着色器
out vec2 v_texCoord;

void main()
{
	gl_Position = position;
	v_texCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

//输入纹理坐标
in vec2 v_texCoord;

uniform vec4 u_color;
uniform sampler2D u_texture;

void main()
{
	//槽，纹理坐标
	vec4 texColor = texture(u_texture, v_texCoord);
	color = texColor;
}
