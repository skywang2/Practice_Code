#version 330 core

out vec4 FragColor;

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * u_lightColor;//环境光
	//漫反射，需要1.着色面法向量；2.光线方向
	vec3 result = ambient * u_objectColor;//环境光作用在物体上，用向量乘法表示，这只是数学上的一种表示，不符合物理
	FragColor = vec4(result, 1.0);
}
