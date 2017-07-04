#ifndef __OPENGL_TEXTURE_DRAW_VIDEO_H__
#define __OPENGL_TEXTURE_DRAW_VIDEO_H__

#include <glew.h>
#include <glut.h>

#include <wglew.h>
#include <windows.h>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glew32s.lib")

#pragma comment(lib, "glut.lib")
#pragma comment(lib, "glut32.lib")

#define ATTRIB_VERTEX	3
#define ATTRIB_TEXTURE	4
#define ID_DRAW_SCENE	WM_USER + 0x1001

class opengl_texture_draw_video : public CWnd
{
public:
	opengl_texture_draw_video();
	~opengl_texture_draw_video();
	
public:
	static opengl_texture_draw_video& Instance();
	static DWORD WINAPI DrawSceneThreadProc(LPVOID lpParam);

protected:
	afx_msg  void			OnPaint();
	afx_msg	 void			OnSize(UINT nType, int cx, int cy);
	afx_msg	 void			OnDraw(CDC *pDC);
	afx_msg	 void			OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

public:
	BOOL					CreateGLContext(CRect rect, CWnd* pParent);
	GLuint					InitContext();

	void					DisplayVideo();
	
protected:
	GLuint					buildshader(const char* pszsource, GLenum shaderType);
	GLuint					buildprogram(const char* vertexShaderSource, const char* fragmentShaderSource);

	GLuint					createsurface();
	
	void					initscene();
	void					drawscene();
	
	GLuint					set_wnd_pixel_format();
	GLuint					create_gl_context();
	GLuint					destroy_gl_context();
	
protected:
	HANDLE					m_hThread;
	
	HANDLE					m_hStartEvent;
	HANDLE					m_hEndEvent;
	
	HDC						m_hDC;
	HGLRC					m_hRC;

	CRect					m_rect;
	CRect					m_oldWindow;
	CRect					m_originalRect;

private:
	BOOL					m_bExit;

	BOOL					m_bIsMaximized;
	GLuint					m_nProgramId;
	
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

	unsigned char*			m_pBuffer;
	unsigned char*  		m_pPlane[3];

	FILE*			m_file;
};

#endif