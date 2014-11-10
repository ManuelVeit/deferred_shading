#version 330

in vec3 vertex_in;
in vec3 normal_in;
in vec2 uv_in;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 vertex_out;
out vec3 normal_out;
out vec3 uv_out;

void main()
{
	vec4 vertexWorldSpace = viewMatrix * vec4(vertex_in, 1.0);
	vec4 vertexClipSpace = projectionMatrix * vertexWorldSpace;

	gl_Position = vertexClipSpace;
	vertex_out = vertexWorldSpace.xyz;
	normal_out = normal_in;
	uv_out = vec3(uv_in, 1.0);
}
