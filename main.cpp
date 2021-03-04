#define GLEW_STATIC
#include "glew.h"
#include "wglew.h"
#include "GLContext.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <stdio.h>
#include "WindowRender.h"
#include "LX_Timer.h"

using namespace std;

#pragma comment(lib,"./glew32s.lib")



int main()
{
	int w = 240, h = 128;
	unsigned char * y = new unsigned char[w*h];
	unsigned char * u = new unsigned char[w*h / 2 / 2];
	unsigned char * v = new unsigned char[w*h / 2 / 2];
	//准备yuv数据
	FILE * fp = fopen("out240x128.yuv","r");
	fread(y, 1, w*h, fp);
	fread(u, 1, w*h / 2 / 2, fp);
	fread(v, 1, w*h / 2 / 2, fp);

	WindowRender wr{ (HWND)0x29149A };

	LX_Timer t;
	for (;;)
	{
		t.start_micro(0);
		wr.UpdateTexture(w,h,y, u, v,w,w/2,w/2);
		wr.RendWindow();
		cout << t.get_micro() << endl;
	}



	return 0;
}







//int main()
//{
//	int w = 800, h = 450;
//	unsigned char * y = new unsigned char[w*h];
//	unsigned char * u = new unsigned char[w*h / 2 / 2];
//	unsigned char * v = new unsigned char[w*h / 2 / 2];
//	//准备yuv数据
//	FILE * fp = fopen("800-450.yuv","r");
//	fread(y, 1, w*h, fp);
//	fread(u, 1, w*h / 2 / 2, fp);
//	fread(v, 1, w*h / 2 / 2, fp);
//	
//
//
//
//
//
//
//	HWND hWnd = (HWND)723112;
//
//
//	GLContext glctx;
//	glctx.setup(hWnd);
//
//	GLenum err = glewInit();
//	if (GLEW_OK != err)
//	{
//		/* Problem: glewInit failed, something is seriously wrong. */
//		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
//	}
//	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
//
//	/*
//	if (wglSwapIntervalEXT != 0)
//	{
//	//为0说明显卡驱动不支持该功能
//	//否则调用该功能
//	wglSwapIntervalEXT(0);//关闭垂直同步 0关闭 1打开
//	}
//	*/
//	
//	
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	//创建纹理
//	//纹理
//	GLuint tex[3] = { 0 };//Y U V
//	glGenTextures(3, tex);
//	//Y
//	glBindTexture(GL_TEXTURE_2D, tex[0]);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, y);
//	//U
//	glBindTexture(GL_TEXTURE_2D, tex[1]);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w / 2, h / 2, 0, GL_RED, GL_UNSIGNED_BYTE, u);
//	//V
//	glBindTexture(GL_TEXTURE_2D, tex[2]);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w / 2, h / 2, 0, GL_RED, GL_UNSIGNED_BYTE, v);
//
//
//
//
//
//
//
//
//
//
//
//	
//
//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	//顶点着色器
//	const char *vertexShaderSource = R"(
//#version 330 core
//layout (location = 0) in vec3 inPos;
//layout (location = 1) in vec2 inTexCoord;
//
//out vec2 outTexCoord;
//
//void main()
//{
//    gl_Position = vec4(inPos.x, inPos.y, inPos.z, 1.0);
//	outTexCoord = inTexCoord;
//}
//
//)";
//
//
//	unsigned int vertexShader;
//	vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//	glCompileShader(vertexShader);
//
//	//编译错误检测
//	{
//		int  success;
//		char infoLog[512];
//		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//		if (!success)
//		{
//			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//		}
//		else
//		{
//			cout << "SHADER::VERTEX::COMPILATION_SUCCESS" << endl;
//		}
//	}
//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	
//	//片段着色器
//	const char* fragmentShaderSource = R"(
//#version 330 core
//
//in vec2 outTexCoord;
//
//uniform sampler2D tex_y;
//uniform sampler2D tex_u;
//uniform sampler2D tex_v;
//
//void main()
//{
//		//R = y + 1.4022 * (v - 128)
//		//G = y - 0.3456 * (u - 128) - 0.7145 * (v - 128)
//		//B = y + 1.771 * (u - 128)
//		vec3 yuv;
//		vec3 rgb;
//
//		//应用矩阵优化后的版本
//		yuv.x = texture2D(tex_y, outTexCoord).r;
//		yuv.y = texture2D(tex_u, outTexCoord).r - 0.5;
//		yuv.z = texture2D(tex_v, outTexCoord).r - 0.5;
//		rgb = mat3(1.0, 1.0, 1.0,
//			0.0, -0.39465, 2.03211,
//			1.13983, -0.58060, 0.0) * yuv;
//
//		gl_FragColor = vec4(rgb, 1.0);
//    
//} 
//	)";
//
//	unsigned int fragmentShader;
//	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//	glCompileShader(fragmentShader);
//
//
//	//编译错误检测
//	{
//		int  success;
//		char infoLog[512];
//		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//		if (!success)
//		{
//			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//			std::cout << "ERROR::SHADER::fragmentShader::COMPILATION_FAILED\n" << infoLog << std::endl;
//		}
//		else
//		{
//			cout << "SHADER::fragmentShader::COMPILATION_SUCCESS" << endl;
//		}
//	}
//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//	//链接着色器
//	unsigned int shaderProgram;
//	shaderProgram = glCreateProgram();
//	glAttachShader(shaderProgram, vertexShader);
//	glAttachShader(shaderProgram, fragmentShader);
//	glLinkProgram(shaderProgram);
//
//	//检查链接
//	{
//		int  success;
//		char infoLog[512];
//		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//		if (!success) {
//			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//			std::cout << "link failed:\n" << infoLog << std::endl;
//		}
//		else
//		{
//			cout << "link success" << endl;
//		}
//	}
//	
//	//在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了：
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//
//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
//
//	unsigned int VAO;
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//
//	float vertices[] = {
//		-0.5f, -0.5f, 0.0f,0,0,
//		-0.5f, 0.5f, 0.0f,0,1,
//		0.5f,  -0.5f, 0.0f,1,0,
//		-0.5f, 0.5f, 0.0f,0,1,
//		0.5f,  -0.5f, 0.0f,1,0,
//		0.5f,0.5f,0.0f,1,1
//
//	};
//
//	unsigned int VBO;
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//
//
//
//
//	
//	//绑定纹理到纹理单元,
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, tex[0]);
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, tex[1]);
//	glActiveTexture(GL_TEXTURE2);
//	glBindTexture(GL_TEXTURE_2D, tex[2]);
//
//	//将纹理单元与着色器中的uniform采样器对应
//	glUseProgram(shaderProgram);
//	glUniform1i(glGetUniformLocation(shaderProgram, "tex_y"), 0);
//	glUniform1i(glGetUniformLocation(shaderProgram, "tex_u"), 1);
//	glUniform1i(glGetUniformLocation(shaderProgram, "tex_v"), 2);
//
//
//
//
//	glEnable(GL_DEPTH_TEST);
//	for (;;)
//	{
//		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//
//		RECT rect;
//		GetClientRect(hWnd, &rect);
//
//
//		glViewport(0, 0, rect.right-rect.left, rect.bottom - rect.top);
//
//		glUseProgram(shaderProgram);
//		glBindVertexArray(VAO);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		glctx.swapBuffer();
//	}
//
//
//	glctx.shutdown();
//
//
//	return 0;
//}