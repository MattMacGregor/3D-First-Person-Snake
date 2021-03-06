#version 330 core

in vec3 Normal;
in vec3 FragPos;
struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
};
struct PointLight
{
	vec3 position;
	
	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct SpotLight 
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	
	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform PointLight pointLight[4];
uniform SpotLight spotLight;
uniform vec3 viewPos;
uniform bool dark;
out vec4 FragColor;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);
	
	float diff = max(dot(normal, lightDir), 0.0);
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * vec3(1.0f, 0.0f, 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(1.0f, 0.0f, 0.0f);
	vec3 specular = light.specular * spec * vec3(1.0f, 1.0f, 1.0f);
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}
	
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);
	
	float diff = max(dot(normal, lightDir), 0.0);
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	
	vec3 ambient = light.ambient * vec3(1.0f, 0.0f, 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(1.0f, 0.0f, 0.0f);
	vec3 specular = light.specular * spec * vec3(1.0f, 1.0f, 1.0f);
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}
void main()
{
	//props
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	vec3 result;
	if(!dark)
	{
		for(int i = 0; i < 4; i++)
			result += CalcPointLight(pointLight[i], norm, FragPos, viewDir);
	}
	else 
	{
		result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
	}
	FragColor = vec4(result, 1.0f);
}
