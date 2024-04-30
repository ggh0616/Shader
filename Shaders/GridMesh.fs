#version 330

layout(location=0) out vec4 FragColor;

in vec4 v_Color;
const float c_PI = 3.1415926535;

void main()
{
	FragColor = v_Color;
}
