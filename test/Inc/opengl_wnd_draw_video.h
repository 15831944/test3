#ifndef __OPENGL_WND_DRAW_VIDEO_H__
#define __OPENGL_WND_DRAW_VIDEO_H__

#include <glew.h>
#include <glut.h>

#include <wglew.h>
#include <windows.h>

#include "thread_queue.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glew32s.lib")

#pragma comment(lib, "glut.lib")
#pragma comment(lib, "glut32.lib")

#define ATTRIB_VERTEX	3
#define ATTRIB_TEXTURE	4

typedef enum tagFrame_data_type{
	FRAME_EMPTY_TYPE = 0,
	FRAME_YUV420PTORGB24_TYPE,
	FRAME_YUV420PDRAW_TYPE
}FRAME_DATA_TYPE;

typedef struct tagFrame_data_buffer{
	char*			pFrameData;
	unsigned long	uFrameDataLen;
	unsigned long   uPixelWidth;
	unsigned long   uPixelHeight;
}FRAME_DATA_BUFFER;

class opengl_wnd_draw_video
{
public:
	opengl_wnd_draw_video();
	~opengl_wnd_draw_video();
	
public:
	static opengl_wnd_draw_video& Instance();
	static DWORD WINAPI DrawSceneThreadProc(LPVOID lpParam);

public:
	BOOL					CreateGLContext(FRAME_DATA_TYPE hDataType, CRect rect, HWND hWnd);	
	BOOL					CloseGLProc();

	void					SetProcTimeOver(GLuint nProcTime, GLuint nCloseTime = 500);
	GLuint					SetFrameData(const unsigned char* pFrameData, unsigned long ulDataLen, unsigned long ulVideoWidth, unsigned long ulVideoHeight);
	
protected:
	GLuint					buildshader(const char* pszsource, GLenum shaderType);
	GLuint					buildprogram(const char* vertexShaderSource, const char* fragmentShaderSource);

	GLuint					createsurface();
	GLuint					initcontext();
	
	void					initscene();
	void					drawscene();
	
	GLuint					set_wnd_pixel_format();
	GLuint					create_gl_context();
	GLuint					destroy_gl_context();

	GLuint					getframedata();
	void					displayvideo();
	
	void					CONVERT_YUV420PtoRGB24(unsigned char* yuv_src,unsigned char* rgb_dst,int nWidth,int nHeight);
	
protected:
	HANDLE					m_hThread;
	
	HANDLE					m_hStartEvent;
	HANDLE					m_hEndEvent;
	
	HDC						m_hDC;
	HWND					m_hWnd;
	HGLRC					m_hRC;

	CRect					m_rect;
	FRAME_DATA_TYPE			m_hDataType;

private:
	BOOL					m_bExit;

	BOOL					m_bIsMaximized;
	GLuint					m_nProgramId;
	GLuint					m_nProcTimeOver;
	GLuint					m_nCloseTimeOver;
	
	GLuint					m_nTextureUniformY;
	GLuint					m_nTextureUniformU;
	GLuint					m_nTextureUniformV;

	GLuint					m_nTextureIdY;
	GLuint					m_nTextureIdU;
	GLuint					m_nTextureIdV;

	GLuint					m_nPixelWidth;
	GLuint					m_nPixelHeight;

	int						m_nWndWidth;
	int						m_nWndHeight;
	
	DWORD					m_dwThreadID;
	GLuint					m_nBufferLen;

	unsigned char*			m_pBuffer;
	unsigned char*  		m_pPlane[3];

	thread_queue<FRAME_DATA_BUFFER*> m_threadFrameQueue;
};

#endif