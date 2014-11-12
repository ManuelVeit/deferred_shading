#version 330

in vec3 vertex_in;
in vec3 normal_in;
in vec2 uv_in;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec2 uv_out;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * vec4(vertex_in, 1.0);
	uv_out = uv_in;
}
