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

in vec3 v_fragPos;//片元坐标
in vec3 v_normal;
in vec2 v_texCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform vec3 u_viewPos;//相机位置坐标
uniform Light u_directLight;//平行光源

vec3 CalcDirLight(Light light, vec3 viewDir, Material material);//平行光
//vec3 CalPointLight();//点光源

float near = 0.1; 
float far  = 100.0; 

//将非线性深度值转为世界坐标下的z轴值
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main()
{
	vec3 viewDir = normalize(u_viewPos - v_fragPos);//片元指向镜头

	Material material;
	material.ambient = vec3(texture(texture_diffuse1, v_texCoord));//与漫反射颜色相同，但使用较弱的环境光
	material.diffuse = vec3(texture(texture_diffuse1, v_texCoord));
	material.specular = vec3(texture(texture_specular1, v_texCoord));
	material.normal = v_normal;//texture(texture_normal1, v_texCoord);//使用纹理采样会导致光线效果异常
	material.shininess = 32.0;

//	FragColor = material.diffuse + material.specular;
//	FragColor = vec4(material.diffuse, 1.0);
	FragColor = vec4(CalcDirLight(u_directLight, viewDir, material), 1.0);

	//1.使用gl_FragCoord.z深度值表示颜色，非线性，近处颜色变化快，远处颜色变化慢
//	FragColor = vec4(vec3(gl_FragCoord.z), 1.0);

	//2.使用线性深度值表示颜色，近处黑，远处白，颜色变化均匀
//	float depth = LinearizeDepth(gl_FragCoord.z) / far;//转为线性深度值
//    FragColor = vec4(vec3(depth), 1.0);
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
