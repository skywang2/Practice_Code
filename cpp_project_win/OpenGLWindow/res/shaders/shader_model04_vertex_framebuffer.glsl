#version 330 core

//2DäÖÈ¾

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord;

void main()
{
	gl_Position = vec4(position.x, position.y, 0.0, 1.0);

	//ÎÆÀí×ø±ê
	v_texCoord = texCoord;
}
