#version 330 core

layout(location = 0) out vec4 color;
//uniform sampler2D u_texture;//纹理槽下标

in vec4 v_color;
void main()
{
	//vec4 texColor = texture(u_texture, v_texCoord);	//槽，纹理坐标
	color = vec4(abs(v_color.x)/50, abs(v_color.y)/50, abs(v_color.z)/50, 1.0);
//	color = vec4(1.0);
}
