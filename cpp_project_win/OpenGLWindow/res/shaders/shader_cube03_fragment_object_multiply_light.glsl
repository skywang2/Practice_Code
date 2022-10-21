#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;//反光度，镜面反射中的指数
	sampler2D diffuseMap;//纹理槽下标
	sampler2D specularMap;//高光槽下标
};

struct Light
{
	vec3 direction;//平行光投射方向，所有平面接受的光线方向一致，不需要计算lightDir，直接lightDir = normalize(-light.direction)，负号是因为要转换成片元所在点为起点

	//点光源光强按距离衰减，系数来自经验值-https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
	float constant;//常数项
	float linear;//一次项
	float quadratic;//二次项
};

//聚光效果，手电筒
struct FlashLight
{
	vec3 position;//光源位置，如果用这个替换u_lightPos光源位置将改变
	vec3 direction;
	float cutOff;//用cos值表示可视角度
};

struct DirLight//平行光
{
	vec3 direction;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight//点光源
{
    vec3 position;

    float constant;//衰减系数
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

out vec4 FragColor;

uniform vec3 u_objectColor;//纯色，物体颜色
uniform vec3 u_lightColor;//纯色，灯光颜色
uniform vec3 u_lightPos;//直接定义lightDir可以实现平行光效果
uniform vec3 u_viewPos;
uniform Material u_material;//物体纹理
uniform Material u_lightMaterial;//光源，自定义颜色、纹理
uniform Light u_light;//平行光、点光源
uniform FlashLight u_flashLight;//4.聚光

#define NR_POINT_LIGHTS 1
uniform DirLight u_dirLight;//平行光
uniform PointLight u_pointLights[NR_POINT_LIGHTS];//点光源数组

in vec3 v_fragPos;
in vec3 v_normal;
in vec2 v_texCoord;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);//平行光
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);//点光源

void main()
{
	vec3 norm = normalize(v_normal);//片元法线
	vec3 viewDir = normalize(u_viewPos - v_fragPos);//片元指向镜头

	vec3 result;
	//平行光
	result += CalcDirLight(u_dirLight, norm, viewDir);
	//点光源
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(u_pointLights[i], norm, viewDir, v_fragPos);
	}
	//聚光

	FragColor = vec4(result, 1.0);
}

/*
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
brief 计算平行光产生的环境光、漫反射、镜面反射合成的反射光颜色
param[in] DirLight light，光线结构体
param[in] vec3 normal，片元法线，已标准化
param[in] vec3 viewDir，视线方向
return vec3
*/
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);//片元指向光源
	//漫反射参数
	float diff = max(dot(normal, lightDir), 0.0);//光线方向向量在法线上的投影
	//镜面反射参数
	vec3 reflectDir = reflect(-lightDir, normal);//-lightDir是光源指向片元
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),  u_material.shininess);
	//三种反射
	vec3 ambient = light.ambient * vec3(texture(u_material.diffuseMap, v_texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuseMap, v_texCoord));
	vec3 specular = light.specular * (spec * vec3(texture(u_material.specularMap, v_texCoord)));

	return (ambient + diffuse + specular);
}

/*
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
brief 计算点光源产生的环境光、漫反射、镜面反射合成的反射光颜色
param[in] PointLight light，光线结构体
param[in] vec3 normal，片元法线，已标准化
param[in] vec3 fragPos，片元的世界坐标
param[in] vec3 viewDir，视线方向
return vec3
*/
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	vec3 lightDir = normalize(light.position - fragPos);//片元指向光源
	//漫反射参数
	float diff = max(dot(normal, lightDir), 0.0);//光线方向向量在法线上的投影
	//镜面反射参数
	vec3 reflectDir = reflect(-lightDir, normal);//-lightDir是光源指向片元
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),  u_material.shininess);
	//衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	//三种反射
	vec3 ambient = light.ambient * vec3(texture(u_material.diffuseMap, v_texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuseMap, v_texCoord));
	vec3 specular = u_lightMaterial.specular * (spec * vec3(texture(u_material.specularMap, v_texCoord)));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

