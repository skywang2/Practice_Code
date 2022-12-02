#version 330 core

out vec4 FragColor;

in vec2 v_texCoord;

uniform sampler2D screenTexture;


vec4 Grey(vec4 color)
{
	float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    return vec4(average, average, average, 1.0);
}

const float offset = 1.0 / 300.0;
//偏移量
vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // 左上
    vec2( 0.0f,    offset), // 正上
    vec2( offset,  offset), // 右上
    vec2(-offset,  0.0f),   // 左
    vec2( 0.0f,    0.0f),   // 中
    vec2( offset,  0.0f),   // 右
    vec2(-offset, -offset), // 左下
    vec2( 0.0f,   -offset), // 正下
    vec2( offset, -offset)  // 右下
);

//卷积核
float kernelSharpen[9] = float[](
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
);

vec4 sampleKernel(float kernel[9])
{
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, v_texCoord.st + offsets[i]));//对当前片元及周围8个进行采样
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];//乘以卷积核

    return vec4(col, 1.0);
}

void main()
{
	FragColor = texture(screenTexture, v_texCoord);//无特效
//	FragColor = vec4(vec3(1.0 - texture(screenTexture, v_texCoord)), 1.0);//反相
//	FragColor = Grey(texture(screenTexture, v_texCoord));//灰度
//  FragColor = sampleKernel(kernelSharpen);//锐化
}
