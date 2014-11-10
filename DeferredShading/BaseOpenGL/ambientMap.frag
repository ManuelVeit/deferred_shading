#version 330

uniform sampler2D ambientMap;
uniform sampler2D diffuseMap;
uniform sampler2D bumpMap;

in vec2 uv_out;
in vec4 color_out;

out vec4 color_final;

void main()
{
	vec4 textureColor = texture2D(ambientMap, uv_out);

	color_final = color_out * textureColor;
}
