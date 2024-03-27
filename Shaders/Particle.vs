#version 330

in vec3 a_Position;

uniform float u_Time = 0;
uniform float u_Period = 0;

const vec3 c_StartPos = vec3(-1, 0, 0);
const vec3 c_Velocity = vec3(2.0, 0.0, 0);
const vec3 c_ParaVelocity = vec3(2.0, 2.0, 0);
const vec2 c_2DGravity = vec2(0, -4.9);

const vec3 c_StartPosTri1 = vec3(-1.0, 0, 0);
const vec3 c_StartPosTri2 = vec3(1.0, 0, 0);
const vec3 c_StartPosTri3 = vec3(0, 1.0, 0);

const float c_PI = 3.141592;

void Line()
{	
	float newTime = abs(fract(u_Time / u_Period) - 0.5) * 2.0; // 화면 왕복, fract() 소수점만 남기기
	vec4 newPosition;
	newPosition.xyz = 
	(c_StartPos + a_Position) +
	c_Velocity * newTime;
	newPosition.w = 1;
	gl_Position = newPosition;
}

void Circle()
{
	float newTime = fract(u_Time / u_Period) * c_PI * 2.0; // 시계 방향으로 돌려면 - 붙이기
	vec2 trans = vec2(cos(newTime), sin(newTime));
	vec4 newPosition;
	newPosition.xy = a_Position.xy + trans;
	newPosition.zw = vec2(0, 1);
	gl_Position = newPosition;
}

void Parabola()
{
	float newTime = fract(u_Time / u_Period);
	float t = newTime;
	float tt = t * t;
	vec4 newPosition;
	float transX = (a_Position.x + c_StartPos.x) 
					+ c_ParaVelocity.x * newTime
					+ 0.5 * c_2DGravity.x * tt;
	float transY = (a_Position.y + c_StartPos.y) 
					+ c_ParaVelocity.y * newTime
					+ 0.5 * c_2DGravity.y * tt;
	newPosition.xy = vec2(transX, transY);
	newPosition.zw = vec2(0, 1);
	gl_Position = newPosition;
}

void Triangle()
{   
	// newPosition.xyz = 
	// (c_StartPos + a_Position) +
	// c_Velocity * newTime;
	// newPosition.w = 1;
	// gl_Position = newPosition;
	// 
	// bool check = false;
    // float newTime = abs(fract(u_Time / u_Period) - 0.5) * 2.0; // 화면 왕복, fract() 소수점만 남기기
	// // if (newTime < -0.000009)
	// // 	check = true;
	// // if (newTime > 0.099999)
	// // 	check = false;
	// vec4 newPosition;
	// if (!check) {
	// 	newPosition.x = 
	// 	(c_StartPos.x + a_Position.x) +
	// 	c_Velocity.x * newTime;
	// 	newPosition.y = 0.0;		
	// }
	// else if (check && newTime < 0 && newTime > -1) {
	// 	newPosition.x = 
	// 	(c_StartPos.x + a_Position.x) +
	// 	c_Velocity.x * newTime;
	// 	newPosition.y = (c_StartPos.y + a_Position.y) +
	// 	(c_Velocity.y + 1.0f) * newTime;
	// }
	// else if (check && newTime > 0 && newTime < 1) {		
	// 	newPosition.x = 
	// 	(c_StartPos.x + a_Position.x) +
	// 	c_Velocity.x * newTime;
	// 	newPosition.y = (c_StartPos.y + a_Position.y) +
	// 	(c_Velocity.y - 1.0f) * newTime;
	// }
	// newPosition.zw = vec2(0, 1);
	// gl_Position = newPosition;
}

void Basic()
{
	vec4 newPosition = vec4(a_Position, 1);
	gl_Position = newPosition;
}

void main()
{
	// Line();
	// Circle();
	// Parabola();
	// Triangle();
	// Basic();
}
