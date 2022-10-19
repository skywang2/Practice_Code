#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
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

out vec4 FragColor;

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;
uniform vec3 u_lightPos;//直接定义lightDir可以实现平行光效果
uniform vec3 u_viewPos;
uniform Material u_material;
uniform Material u_lightMaterial;
uniform Light u_light;//平行光、点光源
uniform FlashLight u_flashLight;//4.聚光

in vec3 v_fragPos;
in vec3 v_normal;
in vec2 v_texCoord;

void main()
{
	//点光源，光强按距离衰减
	float distance = length(u_lightPos - v_fragPos);
	float attenuation = 1.0 / (u_light.constant + u_light.linear * distance + u_light.quadratic * (distance * distance));

	//环境光
	float ambientStrength = 0.1;//相当于I/r^2
//	vec3 ambient = ambientStrength * u_lightColor;//1.直接定义光强（光线颜色）和物体颜色
//	vec3 ambient = u_lightMaterial.ambient * u_material.ambient;//2.使用material
	vec3 ambient = u_lightMaterial.ambient * vec3(texture(u_material.diffuseMap, v_texCoord));//3.使用diffuse map，ambient与diffuse颜色相同

	//漫反射，需要1.着色面法向量；2.光线方向
	vec3 norm = normalize(v_normal);
	vec3 lightDir = normalize(u_lightPos - v_fragPos);
	float diff = max(dot(norm, lightDir), 0.0);//dot，dot product
//	vec3 diffuse = u_lightColor * diff;//1.===注意这个乘法，这是element-wise product===
//	vec3 diffuse = u_lightMaterial.diffuse * (diff * u_material.diffuse);//2.使用material
	vec3 diffuse = u_lightMaterial.diffuse * attenuation * diff * vec3(texture(u_material.diffuseMap, v_texCoord));//3.使用diffuse map
	
	//镜面反射
	float specularStrength = 0.5;//相当于I/r^2
	vec3 viewDir = normalize(u_viewPos - v_fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);//reflect是shader中计算反射方向的
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);//1.使用反射方向与相机方向，与games101中使用半程向量不同
//	vec3 specular = u_lightColor * specularStrength * spec;//1.
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0),  u_material.shininess);//2.
//	vec3 specular = u_lightMaterial.specular * (spec * u_material.specular);//2.使用material
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),  u_material.shininess);//3.
	vec3 specular = u_lightMaterial.specular * attenuation * (spec * vec3(texture(u_material.specularMap, v_texCoord)));//3.使用specular map
	
	//聚光效果，角度范围内有正常光照，角度范围外只有环境光
//	float theta = dot(lightDir, normalize(-u_flashLight.direction));//4.1实际光照(光线从白色立方体出发)与聚光方向夹角的cos
	float theta = dot(normalize(u_viewPos - v_fragPos), normalize(-u_flashLight.direction));//4.2实际光照(光线从镜头方向出发)与聚光方向夹角的cos
	//4.2由于夹角只限制显示范围，且相机并不是真正光源，没有参与三种反射的计算，所以物体靠近相机而光源远离相机时，物体靠近相机的一侧是暗的

	//u_objectColor相当于公式里的反射率k
//	vec3 result = u_objectColor * (ambient + diffuse + specular);//1.光作用在物体上，用wise product表示，这只是数学上的一种表示，只是一种近似
//	vec3 result = ambient + diffuse + specular;//2.使用material物体颜色已经包含在material中；3.使用高光纹理
	vec3 result;
	if(theta > u_flashLight.cutOff)//4.使用聚光
	{
		result = ambient + diffuse + specular;
	}
	else
	{
		result = ambient;
	}

	FragColor = vec4(result, 1.0);
}
