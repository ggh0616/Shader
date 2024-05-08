#version 330

in vec3 a_Position;
out vec4 v_Color;
uniform float u_Time;

const float c_PI = 3.1415926535;

void Flag()
{
	vec4 newPosition = vec4(a_Position, 1);
	float scaledValue = a_Position.x + 0.5;
	float period = 2;	// 林扁
	float timeScale = 6;
	float sinInput = period * scaledValue * 2 * c_PI;
	float sinValue = sin(sinInput - u_Time * timeScale);
	float width = 0.5 * (1 - scaledValue);	// 气

	newPosition.y = newPosition.y * width + scaledValue * width * sinValue;

	v_Color = vec4((sinValue + 1) / 2);
	gl_Position = newPosition;
}

void Flag1()
{
	vec4 newPosition = vec4(a_Position, 1);
	float scaledValue = a_Position.x + 0.5;
	float period = 2;	// 林扁
	float timeScale = 6;
	float sinInput = period * scaledValue * 2 * c_PI;
	float sinValue = sin(sinInput - u_Time * timeScale);
	float width = 0.5 * (1 - scaledValue);	// 气

	newPosition.y = newPosition.y * width + scaledValue * width * sinValue;

	v_Color = vec4((sinValue + 1) / 2);
	gl_Position = newPosition;
}

void Circle()
{
	float r = 5;
	float period = 2;
	float timeScale = 5;
	vec2 circleCenter = vec2(0.5, 0.5);

	float newX = r * sin(a_Position.x * period);
	float newY = r * sin(a_Position.y * period);

	float newTime = fract(u_Time / timeScale);

	vec4 newPosition = vec4(newX, newY, 0, 1);

	vec4 CirclePosition = vec4(a_Position, 1) + newPosition * newTime;

	gl_Position = CirclePosition;
	v_Color = vec4(1);
}

void SphereMapping()
{
	float radius = 1;
	float scale = 5;
	float newX = radius * sin(a_Position.y * scale) * cos(a_Position.x * scale);
	float newY = radius * sin(a_Position.y * scale) * sin(a_Position.x * scale);
	float newZ = radius * cos(a_Position.y * scale);

	float newTime = fract(u_Time / 5);
	vec4 newPosition = vec4(newX, newY, newZ, 1);

	vec4 interResult = vec4(a_Position, 1) * (1 - newTime) + newPosition * newTime;

	gl_Position = interResult;
	v_Color = vec4(1);
}

void main()
{
	// Flag();
	Circle();
	// SphereMapping();
}
