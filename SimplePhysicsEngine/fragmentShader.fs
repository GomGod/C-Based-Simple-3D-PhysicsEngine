#version 330 core
in vec3 Normal;
in vec3 VertexPos;
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	float ambientStrength = 0.5f;
	vec3 ambient = lightColor * ambientStrength;	

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - VertexPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - VertexPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	
	vec3 result = (diffuse+ambient+specular) * objectColor;

	FragColor = vec4(result, 1.0);
}