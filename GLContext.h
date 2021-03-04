#pragma once

#include <windows.h>
#include <gl/gl.h>

#pragma comment (lib,"OpenGL32.Lib")
#pragma comment (lib,"glu32.Lib")
class   GLContext
{
public:
    int         _format;
    //! ���ھ��
    HWND        _hWnd;
    //! �����豸������
    HDC         _hDC;
    //! OpenGL������
    HGLRC       _hRC;
public:
	GLContext();


	~GLContext();

    /**
    *   ��ʼ��GL
    */
	bool    setup(HWND hWnd);
 

    /**
    *   ����EGL
    */
	void    shutdown();
    /**
    *   ����������
    */
	void    swapBuffer();
};
