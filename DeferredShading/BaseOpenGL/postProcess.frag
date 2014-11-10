#version 330

uniform sampler2D ambientMap;

in vec2 uv_out;

out vec4 color_final;

const float PI = 3.1415926535;

void main()
{
	vec4 textureColor = texture2D(ambientMap, uv_out);

	float apertureHalf = 0.5 * 90 * (PI / 180.0);
	float maxFactor = sin(apertureHalf);
  
	vec2 pos = 2.0 * uv_out - 1.0;
	vec2 uv = uv_out;
  
	float l = length(pos);

	if (l <= 0.5)
	{
		float x = maxFactor * pos.x;
		float y = maxFactor * pos.y;
    
		float n = length(vec2(x, y));
    
		float z = sqrt(1.0 - n * n);
  
		float r = atan(n, z) / PI; 
  
		float phi = atan(y, x);

		uv.x = r * cos(phi) + 0.5;
		uv.y = r * sin(phi) + 0.5;
	}

	color_final = texture2D(ambientMap, uv);
}
