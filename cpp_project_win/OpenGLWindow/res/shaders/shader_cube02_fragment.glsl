#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;
in vec4 v_color;

uniform sampler2D u_texture;//纹理槽下标

void main()
{
	vec4 texColor = texture(u_texture, v_texCoord);	//槽，纹理坐标
	color = texColor;
	//color = texColor;
}
