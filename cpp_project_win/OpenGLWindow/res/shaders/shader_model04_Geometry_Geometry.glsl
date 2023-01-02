//几何着色器测试，几何着色器，生成绘制顶点法线的新顶点信息
#version 330 core

layout (triangles) in;//入参是三角形图元
layout (line_strip, max_vertices = 6) out;//输出是多个线段，最多输出6个顶点

in VS_OUT {
    vec3 normal;
} gs_in[];//从顶点着色器传来的接口块

uniform mat4 u_projection;

const float MAGNITUDE = 0.4;//法线放大倍率，用于绘制

void GenerateLine(int index)
{
	//gl_in记录图元中每个顶点的信息
	gl_Position = u_projection * gl_in[index].gl_Position;
    EmitVertex();

    gl_Position = u_projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    GenerateLine(0); // 第一个顶点法线
    GenerateLine(1); // 第二个顶点法线
    GenerateLine(2); // 第三个顶点法线
}
