#version 330 core

layout(location = 0) in vec4 position;
uniform mat4 u_MVP;

//�����Ƭ����ɫ��
out vec4 v_color;
void main()
{
	gl_Position = u_MVP * vec4(position.x, position.y, position.z, 1.0);
	v_color = position;
}
