#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	sampler2D diffuseMap;//�������±�
};

out vec4 FragColor;

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform Material u_material;
uniform Material u_lightMaterial;

in vec3 v_fragPos;
in vec3 v_normal;
in vec2 v_texCoord;

void main()
{
	//������
	float ambientStrength = 0.1;//�൱��I/r^2
//	vec3 ambient = ambientStrength * u_lightColor;//1.ֱ�Ӷ����ǿ��������ɫ����������ɫ
//	vec3 ambient = u_lightMaterial.ambient * u_material.ambient;//2.ʹ��material
	vec3 ambient = u_lightMaterial.ambient * vec3(texture(u_material.diffuseMap, v_texCoord));//3.ʹ��diffuse map��ambient��diffuse��ɫ��ͬ

	//�����䣬��Ҫ1.��ɫ�淨������2.���߷���
	vec3 norm = normalize(v_normal);
	vec3 lightDir = normalize(u_lightPos - v_fragPos);
	float diff = max(dot(norm, lightDir), 0.0);//dot��dot product
//	vec3 diffuse = u_lightColor * diff;//1.===ע������˷�������element-wise product===
//	vec3 diffuse = u_lightMaterial.diffuse * (diff * u_material.diffuse);//2.ʹ��material
	vec3 diffuse = u_lightMaterial.diffuse * diff * vec3(texture(u_material.diffuseMap, v_texCoord));//3.ʹ��diffuse map
	
	//���淴��
	float specularStrength = 0.5;//�൱��I/r^2
	vec3 viewDir = normalize(u_viewPos - v_fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);//reflect��shader�м��㷴�䷽���
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);//1.ʹ�÷��䷽�������������games101��ʹ�ð��������ͬ
//	vec3 specular = u_lightColor * specularStrength * spec;//1.
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),  u_material.shininess);//2.
	vec3 specular = u_lightMaterial.specular * (spec * u_material.specular);//2.ʹ��material
	
	//u_objectColor�൱�ڹ�ʽ��ķ�����k
//	vec3 result = u_objectColor * (ambient + diffuse + specular);//1.�������������ϣ���wise product��ʾ����ֻ����ѧ�ϵ�һ�ֱ�ʾ��ֻ��һ�ֽ���
	vec3 result = ambient + diffuse + specular;//2.ʹ��material������ɫ�Ѿ�������material��
	FragColor = vec4(result, 1.0);

	//FragColor = texture(u_material.diffuseMap, v_texCoord);	//�ۣ���������
	FragColor = vec4(1.0);
}