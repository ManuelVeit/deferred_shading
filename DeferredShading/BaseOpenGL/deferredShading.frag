#version 330

uniform sampler2D ambientMap;

in vec3 vertex_out;
in vec3 normal_out;
in vec3 uv_out;

layout (location = 0) out vec3 vertexWorldSpaceOut;
layout (location = 1) out vec3 normalOut;
layout (location = 2) out vec3 texCoordOut; 
layout (location = 3) out vec3 albedoOut;

const float PI = 3.1415926535;

void main()
{
	vertexWorldSpaceOut = vertex_out;
	normalOut = normal_out;
	texCoordOut = uv_out;
	albedoOut = texture2D(ambientMap, uv_out.st).rgb;
}
