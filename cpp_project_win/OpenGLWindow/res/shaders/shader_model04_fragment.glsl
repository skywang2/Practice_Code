#version 330 core

out vec4 FragColor;

in vec3 v_fragPos;
in vec3 v_normal;
in vec2 v_texCoord;

uniform sampler2D texture_diffuse1;

void main()
{
	FragColor = texture(texture_diffuse1, v_texCoord);
//	FragColor = texture(texture_diffuse1, vec2(10.0, 10.0));
//	FragColor = vec4(1.0);
}
