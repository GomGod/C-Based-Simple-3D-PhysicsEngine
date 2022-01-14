#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 VertexPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec3 nPos = aPos/ 100.0f;
	Normal = aNormal;	
	gl_Position = projection * view * model * vec4(nPos, 1.0);
	VertexPos = vec3(model * vec4(nPos, 1.0));
}