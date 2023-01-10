#version 330 core

//�������Լ���������
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 normal;
	float shininess;//����ȣ����淴���е�ָ��
};

//��Դ
struct Light
{
	vec3 direction;	//���߳��䷽��
	vec3 ambient;		//������
    vec3 diffuse;		//������
    vec3 specular;	//���淴��
};

out vec4 FragColor;

in vec3 v_fragPos;//ƬԪ����
in vec3 v_normal;
in vec2 v_texCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform vec3 u_viewPos;//���λ������
//uniform Light u_directLight;//ƽ�й�Դ

vec3 CalcDirLight(Light light, vec3 viewDir, Material material);//ƽ�й�
//vec3 CalPointLight();//���Դ

void main()
{
	FragColor = vec4(texture(texture_diffuse1, v_texCoord));
}

/*
vec3 CalcDirLight(DirLight light, vec3 viewDir, Material material)
brief ����ƽ�й�����Ļ����⡢�����䡢���淴��ϳɵķ������ɫ
param[in] DirLight light�����߽ṹ��
param[in] vec3 viewDir�����߷���ƬԪָ��ͷ
param[in] Material material��������������
return vec3
*/
vec3 CalcDirLight(Light light, vec3 viewDir, Material material)
{
	vec3 lightDir = normalize(-light.direction);//ƬԪָ���Դ���Թ�Դ����ȡ��
	//���������
	float diff = max(dot(material.normal, lightDir), 0.0);//���߷��������ڷ����ϵ�ͶӰ
	//���淴�����
	vec3 reflectDir = reflect(-lightDir, material.normal);//-lightDir�ǹ�Դָ��ƬԪ
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),  material.shininess);
	//���ַ���
	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;

//	return (ambient + diffuse + specular);
//	return (diffuse + specular);
	return (ambient + diffuse);
}