#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;//����ȣ����淴���е�ָ��
	sampler2D diffuseMap;//�������±�
	sampler2D specularMap;//�߹���±�
};

struct Light
{
	vec3 direction;//ƽ�й�Ͷ�䷽������ƽ����ܵĹ��߷���һ�£�����Ҫ����lightDir��ֱ��lightDir = normalize(-light.direction)����������ΪҪת����ƬԪ���ڵ�Ϊ���

	//���Դ��ǿ������˥����ϵ�����Ծ���ֵ-https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
	float constant;//������
	float linear;//һ����
	float quadratic;//������
};

//�۹�Ч�����ֵ�Ͳ
struct FlashLight
{
	vec3 position;//��Դλ�ã����������滻u_lightPos��Դλ�ý��ı�
	vec3 direction;
	float cutOff;//��cosֵ��ʾ���ӽǶ�
};

struct DirLight//ƽ�й�
{
	vec3 direction;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight//���Դ
{
    vec3 position;

    float constant;//˥��ϵ��
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

out vec4 FragColor;

uniform vec3 u_objectColor;//��ɫ��������ɫ
uniform vec3 u_lightColor;//��ɫ���ƹ���ɫ
uniform vec3 u_lightPos;//ֱ�Ӷ���lightDir����ʵ��ƽ�й�Ч��
uniform vec3 u_viewPos;
uniform Material u_material;//��������
uniform Material u_lightMaterial;//��Դ���Զ�����ɫ������
uniform Light u_light;//ƽ�й⡢���Դ
uniform FlashLight u_flashLight;//4.�۹�

#define NR_POINT_LIGHTS 1
uniform DirLight u_dirLight;//ƽ�й�
uniform PointLight u_pointLights[NR_POINT_LIGHTS];//���Դ����

in vec3 v_fragPos;
in vec3 v_normal;
in vec2 v_texCoord;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);//ƽ�й�
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);//���Դ

void main()
{
	vec3 norm = normalize(v_normal);//ƬԪ����
	vec3 viewDir = normalize(u_viewPos - v_fragPos);//ƬԪָ��ͷ

	vec3 result;
	//ƽ�й�
	result += CalcDirLight(u_dirLight, norm, viewDir);
	//���Դ
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(u_pointLights[i], norm, viewDir, v_fragPos);
	}
	//�۹�

	FragColor = vec4(result, 1.0);
}

/*
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
brief ����ƽ�й�����Ļ����⡢�����䡢���淴��ϳɵķ������ɫ
param[in] DirLight light�����߽ṹ��
param[in] vec3 normal��ƬԪ���ߣ��ѱ�׼��
param[in] vec3 viewDir�����߷���
return vec3
*/
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);//ƬԪָ���Դ
	//���������
	float diff = max(dot(normal, lightDir), 0.0);//���߷��������ڷ����ϵ�ͶӰ
	//���淴�����
	vec3 reflectDir = reflect(-lightDir, normal);//-lightDir�ǹ�Դָ��ƬԪ
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),  u_material.shininess);
	//���ַ���
	vec3 ambient = light.ambient * vec3(texture(u_material.diffuseMap, v_texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuseMap, v_texCoord));
	vec3 specular = light.specular * (spec * vec3(texture(u_material.specularMap, v_texCoord)));

	return (ambient + diffuse + specular);
}

/*
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
brief ������Դ�����Ļ����⡢�����䡢���淴��ϳɵķ������ɫ
param[in] PointLight light�����߽ṹ��
param[in] vec3 normal��ƬԪ���ߣ��ѱ�׼��
param[in] vec3 fragPos��ƬԪ����������
param[in] vec3 viewDir�����߷���
return vec3
*/
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	vec3 lightDir = normalize(light.position - fragPos);//ƬԪָ���Դ
	//���������
	float diff = max(dot(normal, lightDir), 0.0);//���߷��������ڷ����ϵ�ͶӰ
	//���淴�����
	vec3 reflectDir = reflect(-lightDir, normal);//-lightDir�ǹ�Դָ��ƬԪ
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),  u_material.shininess);
	//˥��
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	//���ַ���
	vec3 ambient = light.ambient * vec3(texture(u_material.diffuseMap, v_texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuseMap, v_texCoord));
	vec3 specular = u_lightMaterial.specular * (spec * vec3(texture(u_material.specularMap, v_texCoord)));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}
