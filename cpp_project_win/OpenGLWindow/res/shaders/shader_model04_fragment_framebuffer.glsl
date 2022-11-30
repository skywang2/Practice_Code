#version 330 core

out vec4 FragColor;

in vec2 v_texCoord;

uniform sampler2D screenTexture;

void main()
{
	FragColor = texture(screenTexture, v_texCoord);
}
