//������ɫ�����ԣ�������ɫ�������ɻ��ƶ��㷨�ߵ��¶�����Ϣ
#version 330 core

layout (triangles) in;//�����������ͼԪ
layout (line_strip, max_vertices = 6) out;//����Ƕ���߶Σ�������6������

in VS_OUT {
    vec3 normal;
} gs_in[];//�Ӷ�����ɫ�������Ľӿڿ�

uniform mat4 u_projection;

const float MAGNITUDE = 0.2;//���߷Ŵ��ʣ����ڻ���

void GenerateLine(int index)
{
	//gl_in��¼ͼԪ��ÿ���������Ϣ
	gl_Position = u_projection * gl_in[index].gl_Position;
    EmitVertex();

    gl_Position = u_projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    GenerateLine(0);//��һ�����㷨��
    GenerateLine(1);//�ڶ������㷨��
    GenerateLine(2);//���������㷨��
}