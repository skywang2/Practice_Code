#version 330 core

layout(location = 0) out vec4 color;
//uniform sampler2D u_texture;//�������±�

in vec4 v_color;
void main()
{
//	vec4 texColor = texture(u_texture, v_texCoord);	//�ۣ���������
	color = vec4(v_color.x, v_color.y, v_color.z, 1.0);
//	color = vec4(1.0);
}