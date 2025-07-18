#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 v_texCoord;

void main()
{
	//��պ���������
	v_texCoord = position;

	gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
}
