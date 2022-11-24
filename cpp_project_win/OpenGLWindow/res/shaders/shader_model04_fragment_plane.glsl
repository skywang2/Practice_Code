#version 330 core

out vec4 FragColor;

in vec2 v_texCoord;

uniform sampler2D texture_diffuse1;

void main()
{
//    FragColor = vec4(166/256.0, 231/256.0, 255/256.0, 1.0);
    FragColor = texture(texture_diffuse1,v_texCoord);
}
