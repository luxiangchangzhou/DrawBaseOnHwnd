#pragma once

#include <windows.h>
#include <gl/gl.h>

#pragma comment (lib,"OpenGL32.Lib")
#pragma comment (lib,"glu32.Lib")
class   GLContext
{
public:
    int         _format;
    //! 窗口句柄
    HWND        _hWnd;
    //! 绘制设备上下文
    HDC         _hDC;
    //! OpenGL上下文
    HGLRC       _hRC;
public:
	GLContext();


	~GLContext();

    /**
    *   初始化GL
    */
	bool    setup(HWND hWnd);
 

    /**
    *   销毁EGL
    */
	void    shutdown();
    /**
    *   交换缓冲区
    */
	void    swapBuffer();
};
