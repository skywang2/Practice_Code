#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 v_fragPos;
out vec3 v_normal;
out vec2 v_texCoord;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(position.x, position.y, position.z, 1.0);

	v_fragPos = vec3(u_model * vec4(position, 1.0));

	//mat3(transpose(inverse(u_model)))被称为法线矩阵，将法线转换到世界坐标
	//参考：http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
	v_normal = mat3(transpose(inverse(u_model))) * normal;

	//纹理坐标
	v_texCoord = texCoord;
}
