//������ɫ�����ԣ�������ɫ�������ɻ��ƶ��㷨�ߵ��¶�����Ϣ
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
	gl_Position = u_view * u_model * vec4(position, 1.0);//������ɫ�������ܹ۲�ռ�����

	//mat3(transpose(inverse(u_model)))����Ϊ���߾��󣬽�����ת�����������꣨��۲����꣬ȡ���ڱ������ֻ��M����V*M����P*V*M�任����
	//�ο���http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
	mat3 normalMatrix = mat3(transpose(inverse(u_view * u_model)));
    vs_out.normal = normalize(vec3(vec4(normalMatrix * normal, 0.0)));
}