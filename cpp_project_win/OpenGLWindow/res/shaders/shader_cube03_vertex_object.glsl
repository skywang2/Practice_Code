#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(position.x, position.y, position.z, 1.0);
}
