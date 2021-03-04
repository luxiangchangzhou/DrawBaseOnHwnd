#pragma once
#include <windows.h>
#include "GLContext.h"
class WindowRender
{
public:
	WindowRender(HWND hWnd);
	void UpdateTexture(int w, int h, unsigned char*y, unsigned char*u, unsigned char*v, int stridey, int strideu, int stridev);
	void RendWindow();
	~WindowRender();

private:
	HWND hWnd;
	GLContext glctx;
	unsigned int shaderProgram;
	unsigned int VAO;
	GLuint tex[3] = { 0 };//Y U V
};

