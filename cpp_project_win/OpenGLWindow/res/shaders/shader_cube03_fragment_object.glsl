#version 330 core

out vec4 FragColor;

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * u_lightColor;//������

	vec3 result = ambient * u_objectColor;//�����������������ϣ��������˷���ʾ����ֻ����ѧ�ϵ�һ�ֱ�ʾ������������
	FragColor = vec4(result, 1.0);
}