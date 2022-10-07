#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

out vec4 FragColor;

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform Material u_material;
uniform Material u_lightMaterial;

in vec3 v_normal;
in vec3 v_fragPos;

//uniform sampler2D u_texture;//纹理槽下标
//in vec2 v_texCoord;


void main()
{
	//环境光
	float ambientStrength = 0.1;//相当于I/r^2
//	vec3 ambient = ambientStrength * u_lightColor;//1.直接定义光强（光线颜色）和物体颜色
	vec3 ambient = u_lightMaterial.ambient * u_material.ambient;//2.使用material

	//漫反射，需要1.着色面法向量；2.光线方向
	vec3 norm = normalize(v_normal);
	vec3 lightDir = normalize(u_lightPos - v_fragPos);
	float diff = max(dot(norm, lightDir), 0.0);//dot，dot product
//	vec3 diffuse = u_lightColor * diff;//1.===注意这个乘法，这是element-wise product===
	vec3 diffuse = u_lightMaterial.diffuse * (diff * u_material.diffuse);//2.使用material
	
	//镜面反射
	float specularStrength = 0.5;//相当于I/r^2
	vec3 viewDir = normalize(u_viewPos - v_fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);//reflect是shader中计算反射方向的
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);//1.使用反射方向与相机方向，与games101中使用半程向量不同
//	vec3 specular = u_lightColor * specularStrength * spec;//1.
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),  u_material.shininess);//2.
	vec3 specular = u_lightMaterial.specular * (spec * u_material.specular);//2.使用material
	
	//u_objectColor相当于公式里的反射率k
//	vec3 result = u_objectColor * (ambient + diffuse + specular);//1.光作用在物体上，用wise product表示，这只是数学上的一种表示，只是一种近似
	vec3 result = ambient + diffuse + specular;//2.使用material物体颜色已经包含在material中
	FragColor = vec4(result, 1.0);

//	FragColor = texture(u_texture, v_texCoord);	//槽，纹理坐标

}
