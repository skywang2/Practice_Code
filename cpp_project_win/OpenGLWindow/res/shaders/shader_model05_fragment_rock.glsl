#version 330 core

//物体属性及纹理采样
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 normal;
	float shininess;//反光度，镜面反射中的指数
};

//光源
struct Light
{
	vec3 direction;	//光线出射方向
	vec3 ambient;		//环境光
    vec3 diffuse;		//漫反射
    vec3 specular;	//镜面反射
};

out vec4 FragColor;

in vec2 v_texCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform vec3 u_viewPos;//相机位置坐标

vec3 CalcDirLight(Light light, vec3 viewDir, Material material);//平行光

void main()
{
	FragColor = vec4(texture(texture_diffuse1, v_texCoord));
}

/*
vec3 CalcDirLight(DirLight light, vec3 viewDir, Material material)
brief 计算平行光产生的环境光、漫反射、镜面反射合成的反射光颜色
param[in] DirLight light，光线结构体
param[in] vec3 viewDir，视线方向，片元指向镜头
param[in] Material material，纹理采样集合
return vec3
*/
vec3 CalcDirLight(Light light, vec3 viewDir, Material material)
{
	vec3 lightDir = normalize(-light.direction);//片元指向光源，对光源方向取反
	//漫反射参数
	float diff = max(dot(material.normal, lightDir), 0.0);//光线方向向量在法线上的投影
	//镜面反射参数
	vec3 reflectDir = reflect(-lightDir, material.normal);//-lightDir是光源指向片元
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),  material.shininess);
	//三种反射
	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;

//	return (ambient + diffuse + specular);
//	return (diffuse + specular);
	return (ambient + diffuse);
}
