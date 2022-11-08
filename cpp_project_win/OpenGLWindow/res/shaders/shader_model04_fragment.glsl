#version 330 core

out vec4 FragColor;

in vec3 v_fragPos;
in vec3 v_normal;
in vec2 v_texCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

void main()
{
	vec4 diffuse = texture(texture_diffuse1, v_texCoord);
	vec4 specular = texture(texture_specular1, v_texCoord);
	vec4 normal = texture(texture_normal1, v_texCoord);

	FragColor = diffuse + specular;
//	FragColor = diffuse;
//	FragColor = specular;
//	FragColor = normal;
//	FragColor = vec4(1.0);
}
