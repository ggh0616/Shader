#version 330

layout(location=0) out vec4 FragColor;

uniform sampler2D u_TextureSampler;
uniform sampler2D u_NumberTexture[10];
uniform sampler2D u_NumbersTexture;
uniform float u_Time;

const float c_PI = 3.14159263;

in vec2 v_TexPos;

void p0()
{
	// FragColor = vec4(v_TexPos, 0, 1);
	float tx = v_TexPos.x;			 
	float ty = fract(v_TexPos.y);
	vec2 newTexPos = vec2(tx, ty);
	FragColor = texture(u_TextureSampler, newTexPos);
}

void p1()
{
	// FragColor = vec4(v_TexPos, 0, 1);
	float tx = v_TexPos.x;			 
	float ty = fract(v_TexPos.y * 2);
	vec2 newTexPos = vec2(tx, ty);
	FragColor = texture(u_TextureSampler, newTexPos);
}

void p2()
{
	// FragColor = vec4(v_TexPos, 0, 1);
	float tx = v_TexPos.x;			 
	float ty = 1 - abs(v_TexPos.y * 2 - 1);
	vec2 newTexPos = vec2(tx, ty);
	FragColor = texture(u_TextureSampler, newTexPos);
}

void p3()
{
	// FragColor = vec4(v_TexPos, 0, 1);
	float tx = fract(v_TexPos.x * 3);	// 0-1, 0-1, 0-1			 
	float ty = v_TexPos.y / 3 + (2 - floor(v_TexPos.x * 3)) / 3;	// 2/3 ~ 3/3, 1/3 ~ 2/3, 0 ~ 1/3
	vec2 newTexPos = vec2(tx, ty);
	FragColor = texture(u_TextureSampler, newTexPos);
	// FragColor = vec4(ty);
}

void p4()
{
	// FragColor = vec4(v_TexPos, 0, 1);
	float tx = fract(v_TexPos.x * 3);	// 0-1, 0-1, 0-1			 
	float ty = v_TexPos.y / 3 + (floor(v_TexPos.x * 3)) / 3;	// 0 ~ 1/3,  1/3 ~ 2/3, 2/3 ~ 3/3
	vec2 newTexPos = vec2(tx, ty);
	FragColor = texture(u_TextureSampler, newTexPos);
	// FragColor = vec4(ty);
}

void p5()
{
	float tx = fract(v_TexPos.x);		// 0-1		 
	float ty = (v_TexPos.y / 3 + (2 - floor(v_TexPos.y * 3)) / 3);	// 2/3 ~ 3/3, 1/3 ~ 2/3, 0 ~ 1/3
	vec2 newTexPos = vec2(tx, ty);
	FragColor = texture(u_TextureSampler, newTexPos);
	// FragColor = vec4(tx, ty, 0, 1);
}

void p6()
{
	float padding = 0.5 + u_Time;
	float countX = 2;
	float countY = 2;
	float tx = fract((floor(v_TexPos.y *  countY)) * padding) + v_TexPos.x * countX;		 
	float ty = fract(v_TexPos.y * countY);
	vec2 newTexPos = vec2(tx, ty);
	FragColor = texture(u_TextureSampler, newTexPos);
}

void p7()
{
	float padding = 0.5 + u_Time;
	float countX = 2;
	float countY = 2;
	float tx = fract(v_TexPos.x * countX);		 
	float ty = fract((floor(v_TexPos.x * countX)) * padding) + v_TexPos.y * countY;
	vec2 newTexPos = vec2(tx, ty);
	FragColor = texture(u_TextureSampler, newTexPos);
}

void p8()
{
	float xResol = 5;
	float yResol = 2;
	float id = 5;
	float indexX = float(int(id) % int(xResol));
	float indexY = floor(id / xResol);
	float tx = v_TexPos.x / xResol + indexX * (1 / xResol);	// 0~0.2			 
	float ty = v_TexPos.y / yResol + indexY * (1 / yResol);	// 0~0.5
	vec2 newTexPos = vec2(tx, ty);

	// int texID = int(u_Time) % 10;

	FragColor = texture(u_NumbersTexture, newTexPos);
}

void main()
{
	// p0();
	// p1();
	// p2();
	// p3();
	// p4();
	p5();
	// p6();
	// p7();
	// p8();
}
