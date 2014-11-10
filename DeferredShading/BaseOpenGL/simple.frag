#version 330

in vec2 uv_out;
in vec4 color_out;

out vec4 color_final;

void main()
{
	color_final = color_out;
}
