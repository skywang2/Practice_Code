//几何着色器测试，顶点着色器，生成绘制顶点法线的新顶点信息
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out VS_OUT {
    vec3 normal;
} vs_out;

void main()
{
	gl_Position = u_view * u_model * vec4(position, 1.0);//几何着色器将接受观察空间坐标

	//mat3(transpose(inverse(u_model)))被称为法线矩阵，将法线转换到世界坐标（或观察坐标，取决于被计算的只有M还是V*M或者P*V*M变换矩阵）
	//参考：http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
	mat3 normalMatrix = mat3(transpose(inverse(u_view * u_model)));
    vs_out.normal = normalize(vec3(vec4(normalMatrix * normal, 0.0)));
}
