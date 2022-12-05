#version 330 core

out vec4 FragColor;

in vec3 v_texCoord;//ע��vec3

uniform samplerCube skybox;

void main()
{
	FragColor = texture(skybox, v_texCoord);//����Ч
}
