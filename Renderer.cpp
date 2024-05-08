#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_ParticleShader = CompileShaders("./Shaders/Particle.vs", "./Shaders/Particle.fs");
	m_ParticleCloudShader = CompileShaders("./Shaders/ParticleCloud.vs", "./Shaders/ParticleCloud.fs");
	m_FSSandboxShader = CompileShaders("./Shaders/FSSandbox .vs", "./Shaders/FSSandbox .fs");
	m_GridMeshShader = CompileShaders("./Shaders/GridMesh.vs", "./Shaders/GridMesh.fs");
	m_TextureSandboxShader = CompileShaders("./Shaders/TextureSandbox.vs", "./Shaders/TextureSandbox.fs");
	m_TextureShader = CompileShaders("./Shaders/Texture.vs", "./Shaders/Texture.fs");
	m_AlphaClearShader = CompileShaders("./Shaders/AlphaClear.vs", "./Shaders/AlphaClear.fs");
	
	// Create VBOs
	CreateVertexBufferObjects();

	// Create Particle Cloud
	CreateParticleCloud(10000);

	// Create GridMesh
	CreateGridMesh(32, 32);

	// Create Texture
	m_RGBTexture = CreatePngTexture("./rgb.png", GL_NEAREST);
	// m_NumberTexture[0] = CreatePngTexture("./Textures/0.png", GL_NEAREST);
	// m_NumberTexture[1] = CreatePngTexture("./Textures/1.png", GL_NEAREST);
	// m_NumberTexture[2] = CreatePngTexture("./Textures/2.png", GL_NEAREST);
	// m_NumberTexture[3] = CreatePngTexture("./Textures/3.png", GL_NEAREST);
	// m_NumberTexture[4] = CreatePngTexture("./Textures/4.png", GL_NEAREST);
	// m_NumberTexture[5] = CreatePngTexture("./Textures/5.png", GL_NEAREST);
	// m_NumberTexture[6] = CreatePngTexture("./Textures/6.png", GL_NEAREST);
	// m_NumberTexture[7] = CreatePngTexture("./Textures/7.png", GL_NEAREST);
	// m_NumberTexture[8] = CreatePngTexture("./Textures/8.png", GL_NEAREST);
	// m_NumberTexture[9] = CreatePngTexture("./Textures/9.png", GL_NEAREST);	
	// m_NumbersTexture = CreatePngTexture("./Textures/numbers.png", GL_NEAREST);
	m_ParticleTexture = CreatePngTexture("./Textures/cleanParticle.png", GL_NEAREST);

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}

	//Create FBO
	CreateFBOs();
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float vertices[] = { 
		0.f, 0.f, 0.f, 
		1.f, 0.f, 0.f, 
		1.f, 1.f, 0.f 
	};

	glGenBuffers(1, &m_VBOTest);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTest);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	float size = 0.05f;
	float particles[] = {
		-size, -size, 0.f,
		 size,  size, 0.f,
		-size,  size, 0.f,
		-size, -size, 0.f,
		 size, -size, 0.f,
		 size,  size, 0.f		
	};
	// float* a = new float[100];
	glGenBuffers(1, &m_ParticleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_STATIC_DRAW);

	size = 0.5f;
	float FSSandboxVerts[] = {
		-size, -size, 0.f,
		 size,  size, 0.f,
		-size,  size, 0.f,
		-size, -size, 0.f,
		 size, -size, 0.f,
		 size,  size, 0.f
	};
	glGenBuffers(1, &m_FSSandboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_FSSandboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FSSandboxVerts), FSSandboxVerts, GL_STATIC_DRAW);

	size = 0.5f;
	float TextureSandboxVerts[] = {
		-size, -size, 0, 0, 1,
		 size,  size, 0, 1, 0,
		-size,  size, 0, 0, 0,
		-size, -size, 0, 0, 1,
		 size, -size, 0, 1, 1,
		 size,  size, 0, 1, 0
	};
	glGenBuffers(1, &m_TextureSandboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TextureSandboxVerts), TextureSandboxVerts, GL_STATIC_DRAW);

	float TextureVerts[] = {
		 0, 0, 0,
		 1, 1, 0, 
		 0, 1, 0, 
		 0, 0, 0, 
		 1, 0, 0, 
		 1, 1, 0, 
	};
	glGenBuffers(1, &m_TextureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TextureVerts), TextureVerts, GL_STATIC_DRAW);

	
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawTest()
{
	//Program select
	glUseProgram(m_SolidRectShader);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTest);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::DrawParticle()
{
	//Program select
	GLuint shader = m_ParticleShader;
	glUseProgram(shader);

	GLuint uniformTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uniformTime, m_ParticleTime);
	m_ParticleTime += 0.01f;

	GLuint uniformPeriod = glGetUniformLocation(shader, "u_Period");
	glUniform1f(uniformPeriod, 5.0f);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::DrawParticleCloud()
{
	// Alpha Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);

	//Program select
	GLuint shader = m_ParticleCloudShader;
	glUseProgram(shader);
	GLuint stride = sizeof(float) * 17;

	int uniformTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uniformTime, m_ParticleCloudTime);
	m_ParticleCloudTime += 0.016f;

	int uniformPeriod = glGetUniformLocation(shader, "u_Period");
	glUniform1f(uniformPeriod, 2.0f);

	int uniformAcc = glGetUniformLocation(shader, "u_Acc");
	glUniform2f(uniformAcc, cos(m_ParticleCloudTime / 10), sin(m_ParticleCloudTime / 10));

	int uniformTexture = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uniformTexture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ParticleTexture);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribPosition, 
		3, GL_FLOAT, GL_FALSE, 
		stride, 0);

	int attribVelocity = glGetAttribLocation(shader, "a_Velocity");
	glEnableVertexAttribArray(attribVelocity);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribVelocity, 
		3, GL_FLOAT, GL_FALSE, 
		stride, (GLvoid*)(sizeof(float) * 3));

	int attribStartTime = glGetAttribLocation(shader, "a_StartTime");
	glEnableVertexAttribArray(attribStartTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribStartTime, 
		1, GL_FLOAT, GL_FALSE, 
		stride, (GLvoid*)(sizeof(float) * 6));

	int attriblifeTime = glGetAttribLocation(shader, "a_lifeTime");
	glEnableVertexAttribArray(attriblifeTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attriblifeTime,
		1, GL_FLOAT, GL_FALSE,
		stride, (GLvoid*)(sizeof(float) * 7));

	int attribAmp = glGetAttribLocation(shader, "a_Amp");
	glEnableVertexAttribArray(attribAmp);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribAmp,
		1, GL_FLOAT, GL_FALSE,
		stride, (GLvoid*)(sizeof(float) * 8));

	int attribPeriod = glGetAttribLocation(shader, "a_Period");
	glEnableVertexAttribArray(attribPeriod);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribPeriod,
		1, GL_FLOAT, GL_FALSE,
		stride, (GLvoid*)(sizeof(float) * 9));

	int attribValue = glGetAttribLocation(shader, "a_Value");
	glEnableVertexAttribArray(attribValue);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribValue,
		1, GL_FLOAT, GL_FALSE,
		stride, (GLvoid*)(sizeof(float) * 10));

	int attribColor = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribColor,
		4, GL_FLOAT, GL_FALSE,
		stride, (GLvoid*)(sizeof(float) * 11));

	int attribTexPos = glGetAttribLocation(shader, "a_TexPos");
	glEnableVertexAttribArray(attribTexPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribTexPos,
		2, GL_FLOAT, GL_FALSE,
		stride, (GLvoid*)(sizeof(float) * 15));

	glDrawArrays(GL_TRIANGLES, 0, m_ParticleCloudVertexCount);
	glDisableVertexAttribArray(attribPosition);
	glDisable(GL_BLEND);
}

void Renderer::DrawFSSandbox()
{
	// Alpha Blending
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);

	//Program select
	GLuint shader = m_FSSandboxShader;
	glUseProgram(shader);
	GLuint stride = sizeof(float) * 3;

	int uniformTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uniformTime, m_FSSandboxTime);
	m_FSSandboxTime += 0.016;

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_FSSandboxVBO);
	glVertexAttribPointer(attribPosition,
		3, GL_FLOAT, GL_FALSE,
		stride, 0);

	glDrawArrays(GL_LINE, 0, 6);
	glDisableVertexAttribArray(attribPosition);
	// glDisable(GL_BLEND);
}

void Renderer::DrawGridMesh()
{
	//Program select
	GLuint shader = m_GridMeshShader;
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	GLuint u_Time = glGetUniformLocation(shader, "u_Time");
	glUniform1f(u_Time, m_GridMeshTime);
	m_GridMeshTime += 0.016;

	glDrawArrays(GL_LINE_STRIP, 0, m_GridMeshVertexCount);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::DrawTextureSandbox()
{
	//Program select
	GLuint shader = m_TextureSandboxShader;
	glUseProgram(shader);
	float stride = sizeof(float) * 5;

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandboxVBO);
	glVertexAttribPointer(attribPosition, 
		3, GL_FLOAT, GL_FALSE, stride, 0);

	GLuint u_Time = glGetUniformLocation(shader, "u_Time");
	glUniform1f(u_Time, m_TextureSandboxTime);
	m_TextureSandboxTime += 0.016;	

	int attribTexture = glGetAttribLocation(shader, "a_Texture");
	glEnableVertexAttribArray(attribTexture);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandboxVBO);
	glVertexAttribPointer(attribTexture, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 3));

	int u_Texture = glGetUniformLocation(shader, "u_TextureSampler");
	glUniform1i(u_Texture, 0);
	//glUniform1i(u_Texture, (int)(m_TextureSandboxTime) % 10);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RGBTexture);

	// int u_Texture1 = glGetUniformLocation(shader, "u_NumberTexture");
	// int textures[] = { 1,2,3,4,5,6,7,8,9, 10 };
	// glUniform1iv(u_Texture1, 10, textures);
	// 
	// int u_Texture2 = glGetUniformLocation(shader, "u_NumbersTexture");
	// glUniform1i(u_Texture2, 11);
	// 
	// for (int i = 0; i < 10; ++i) {
	// 	glActiveTexture(GL_TEXTURE1 + i);
	// 	glBindTexture(GL_TEXTURE_2D, m_NumberTexture[i]);
	// }
	// glActiveTexture(GL_TEXTURE11);
	// glBindTexture(GL_TEXTURE_2D, m_NumbersTexture);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(attribPosition);
}

void Renderer::DrawTexture(float x, float y, float sizeX, float sizeY, GLuint texID)
{
	//Program select
	GLuint shader = m_TextureShader;
	glUseProgram(shader);
	float stride = sizeof(float) * 3;

	int u_ScreenResol = glGetUniformLocation(shader, "u_ScreenResol");
	glUniform2f(u_ScreenResol, (float)m_WindowSizeX, (float)m_WindowSizeY);

	int u_Position = glGetUniformLocation(shader, "u_Position");
	glUniform2f(u_Position, x, y);

	int u_Size = glGetUniformLocation(shader, "u_Size");
	glUniform2f(u_Size, sizeX, sizeY);	

	int u_Texture = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(u_Texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureVBO);
	glVertexAttribPointer(attribPosition,
		3, GL_FLOAT, GL_FALSE, stride, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawAlphaClear(float r, float g, float b, float a)
{
	GLuint shader = m_AlphaClearShader;
	glUseProgram(shader);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint u_Color = glGetUniformLocation(shader, "u_Color");
	glUniform4f(u_Color, r, g, b, a);

	GLuint posLoc = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(posLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_FullRectVBO);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawTotal()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_A_FBO);
	DrawAlphaClear(0, 0, 0, 0.1);
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, DrawBuffers);

	DrawParticleCloud();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	DrawTexture(0, 0, 
			m_WindowSizeX, m_WindowSizeY, 
			m_A_FBOTexture0);

	//glBindFramebuffer(GL_FRAMEBUFFER, m_A_FBO);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glViewport(0, 0, m_WindowSizeX, m_WindowSizeX);

	//// Renderer Test
	//// DrawSolidRect(0, 0, 0, 4, 1, 0, 1, 1);
	//// DrawTest();
	//// DrawParticle();
	// DrawParticleCloud();
	//// DrawFSSandbox();
	//// DrawGridMesh();
	//// DrawTextureSandbox();
	//glViewport(0, 0, m_WindowSizeX, m_WindowSizeX);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//DrawTexture(0, 0, 
	//			m_WindowSizeX / 2, m_WindowSizeY / 2, 
	//	m_A_FBOTexture1);
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

void Renderer::CreateParticleCloud(int numParticles)
{
	float centerX, centerY;
	centerX = 0.f;
	centerY = 0.f;
	float size = 0.005f;
	int particleCount = numParticles;
	int vertexCount = particleCount * 6;
	int floatCount = vertexCount * (3 + 3 + 1 + 1 + 1 + 1 + 1 + 4 + 2);
	// x, y, z, vx, vy, vz, 
	// startTime, lifeTime, 
	// amp, period, value, 
	// r, g, b, a, 
	// tx, ty

	float* vertices = NULL;
	vertices = new float[floatCount];

	float startTime;
	float lifeTime;
	float vx, vy, vz;
	float amp, period;
	float value;
	float r, g, b, a;
	int index = 0;

	for (int i = 0; i < particleCount; ++i) {
		float velocityScale = 0.2f;
		// centerX = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f; // -1.0 ~ 1.0
		// centerY = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f; // -1.0 ~ 1.0
		centerX = 0.f;
		centerY = 0.f;
		vx = 0.f; // (((float)rand() / (float)RAND_MAX) * 2.f - 1.f) * velocityScale; // -1.0 ~ 1.0
		vy = -0.01f; // (((float)rand() / (float)RAND_MAX) * 2.f - 1.f) * velocityScale; // -1.0 ~ 1.0
		vz = (((float)rand() / (float)RAND_MAX) * 2.f - 1.f) * velocityScale; // -1.0 ~ 1.0
		// vx = 0.f;
		// vy = 0.5f;
		// vz = 0.f;
		startTime = 10.f * ((float)rand() / (float)RAND_MAX);
		lifeTime = 1.f * ((float)rand() / (float)RAND_MAX) + 1.f ;
		// startTime = 0.f;
		// lifeTime = 2.f;
		amp = (((float)rand() / (float)RAND_MAX)- 0.5) * 2.f;
		period = ((float)rand() / (float)RAND_MAX);
		value = ((float)rand() / (float)RAND_MAX);
		r = ((float)rand() / (float)RAND_MAX);
		g = ((float)rand() / (float)RAND_MAX);
		b = ((float)rand() / (float)RAND_MAX);
		a = ((float)rand() / (float)RAND_MAX);

		vertices[index] = centerX - size; index++;
		vertices[index] = centerY - size; index++;
		vertices[index] = 0.f;			  index++;
		vertices[index] = vx;			  index++;
		vertices[index] = vy;			  index++;
		vertices[index] = vz;			  index++;
		vertices[index] = startTime;	  index++;
		vertices[index] = lifeTime;		  index++;
		vertices[index] = amp;			  index++;
		vertices[index] = period;		  index++;
		vertices[index] = value;		  index++;
		vertices[index] = r;			  index++;
		vertices[index] = g;			  index++;
		vertices[index] = b;			  index++;
		vertices[index] = a;			  index++;
		vertices[index] = 0;			  index++;
		vertices[index] = 1;			  index++;

		vertices[index] = centerX + size; index++;
		vertices[index] = centerY + size; index++;
		vertices[index] = 0.f;		      index++;
		vertices[index] = vx;			  index++;
		vertices[index] = vy;			  index++;
		vertices[index] = vz;			  index++;
		vertices[index] = startTime;	  index++;
		vertices[index] = lifeTime;		  index++;
		vertices[index] = amp;			  index++;
		vertices[index] = period;		  index++;
		vertices[index] = value;		  index++;
		vertices[index] = r;			  index++;
		vertices[index] = g;			  index++;
		vertices[index] = b;			  index++;
		vertices[index] = a;			  index++;
		vertices[index] = 1;			  index++;
		vertices[index] = 0;			  index++;

		vertices[index] = centerX - size; index++;
		vertices[index] = centerY + size; index++;
		vertices[index] = 0.f;			  index++;
		vertices[index] = vx;			  index++;
		vertices[index] = vy;			  index++;
		vertices[index] = vz;			  index++;
		vertices[index] = startTime;	  index++; 
		vertices[index] = lifeTime;		  index++;
		vertices[index] = amp;			  index++;
		vertices[index] = period;		  index++;
		vertices[index] = value;		  index++;
		vertices[index] = r;			  index++;
		vertices[index] = g;			  index++;
		vertices[index] = b;			  index++;
		vertices[index] = a;			  index++;
		vertices[index] = 0;			  index++;
		vertices[index] = 0;			  index++;// Triangle1

		vertices[index] = centerX - size; index++;
		vertices[index] = centerY - size; index++;
		vertices[index] = 0.f;			  index++;
		vertices[index] = vx;			  index++;
		vertices[index] = vy;			  index++;
		vertices[index] = vz;			  index++;
		vertices[index] = startTime;	  index++;
		vertices[index] = lifeTime;		  index++;
		vertices[index] = amp;			  index++;
		vertices[index] = period;		  index++;
		vertices[index] = value;		  index++;
		vertices[index] = r;			  index++;
		vertices[index] = g;			  index++;
		vertices[index] = b;			  index++;
		vertices[index] = a;			  index++;
		vertices[index] = 0;			  index++;
		vertices[index] = 1;			  index++;

		vertices[index] = centerX + size; index++;
		vertices[index] = centerY - size; index++;
		vertices[index] = 0.f;		      index++;
		vertices[index] = vx;			  index++;
		vertices[index] = vy;			  index++;
		vertices[index] = vz;			  index++;
		vertices[index] = startTime;	  index++;
		vertices[index] = lifeTime;		  index++;
		vertices[index] = amp;			  index++;
		vertices[index] = period;		  index++;
		vertices[index] = value;		  index++;
		vertices[index] = r;			  index++;
		vertices[index] = g;			  index++;
		vertices[index] = b;			  index++;
		vertices[index] = a;			  index++;
		vertices[index] = 1;			  index++;
		vertices[index] = 1;			  index++;

		vertices[index] = centerX + size; index++;
		vertices[index] = centerY + size; index++;
		vertices[index] = 0.f;			  index++;
		vertices[index] = vx;			  index++;
		vertices[index] = vy;			  index++;
		vertices[index] = vz;			  index++;
		vertices[index] = startTime;	  index++; 
		vertices[index] = lifeTime;		  index++;
		vertices[index] = amp;			  index++;
		vertices[index] = period;		  index++;
		vertices[index] = value;		  index++;
		vertices[index] = r;			  index++;
		vertices[index] = g;			  index++;
		vertices[index] = b;			  index++;
		vertices[index] = a;			  index++;
		vertices[index] = 1;			  index++;
		vertices[index] = 0;			  index++;// Triangle2
	}

	glGenBuffers(1, &m_ParticleCloudVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, vertices, GL_STATIC_DRAW);
	m_ParticleCloudVertexCount = vertexCount;
	
	delete[] vertices;
	
	float sizeX = 1.f;
	float sizeY = 1.f;

	float fullRectTextureRect[] =
	{
	-sizeX,  sizeY, 0.f,    0.f, 0.f, //x, y, z, tx, ty
	-sizeX, -sizeY, 0.f,    0.f, 1.f,
	 sizeX,  sizeY, 0.f,    1.f, 0.f, //Triangle1
	 sizeX,  sizeY, 0.f,    1.f, 0.f,
	-sizeX, -sizeY, 0.f,    0.f, 1.f,
	 sizeX, -sizeY, 0.f,    1.f, 1.f, //Triangle2 
	};

	glGenBuffers(1, &m_FullRectVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_FullRectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fullRectTextureRect), fullRectTextureRect, GL_STATIC_DRAW);
}

void Renderer::CreateGridMesh(int x, int y)
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = x;
	int pointCountY = y;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX * pointCountY * 2];
	float* vertices = new float[(pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3];
	m_GridMeshVertexCount = (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y * pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y * pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &m_GridMeshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);

}

GLuint Renderer::CreatePngTexture(char* filePath, GLuint samplingMethod)
{
	//Load Png
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		std::cout << "PNG image loading failed:" << filePath << std::endl;
		// assert(0);
	}

	GLuint temp;
	glGenTextures(1, &temp);
	glBindTexture(GL_TEXTURE_2D, temp);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, &image[0]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMethod);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMethod);

	return temp;	
}

void Renderer::CreateFBOs()
{
	glGenTextures(1, &m_A_FBOTexture0);
	glBindTexture(GL_TEXTURE_2D, m_A_FBOTexture0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glGenTextures(1, &m_A_FBOTexture1);
	glBindTexture(GL_TEXTURE_2D, m_A_FBOTexture1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &m_A_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_A_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_A_FBOTexture0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_A_FBOTexture1, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Gen FBO failed" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

