#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
}; 

struct Light
{
	vec3 lightColor;
	vec3 lightPos;
};

in vec3 Normal;
in vec3 VertexPos;
out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{	
	vec3 ambient = light.lightColor * material.ambient;	

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.lightPos - VertexPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff*material.diffuse) * light.lightColor;
		
	vec3 viewDir = normalize(viewPos - VertexPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = (spec *material.specular )  * light.lightColor;

	
	vec3 result = (diffuse+ambient+specular) * material.color;

	FragColor = vec4(result, 1.0);
}