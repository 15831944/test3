#ifndef __OPENGL_TEXTURE_DRAW_VIDEO_H__
#define __OPENGL_TEXTURE_DRAW_VIDEO_H__

#include <glew.h>
#include <glut.h>
#include <windows.h>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glew32s.lib")

#pragma comment(lib, "glut.lib")
#pragma comment(lib, "glut32.lib")

#define ATTRIB_VERTEX	3
#define ATTRIB_TEXTURE	4

typedef enum tagOpenGL_Wnd_Type
{
	OPENGL_EMPTY_TYPE = 0,
	OPENGL_CONSOLE_TYPE,
	OPENGL_WIN32_TYPE
}OPENGL_WND_TYPE;

class opengl_texture_draw_video
{
public:
	opengl_texture_draw_video();
	~opengl_texture_draw_video();
	
public:
	static opengl_texture_draw_video& Instance();

public:
	GLuint					init_context(OPENGL_WND_TYPE hWndType);

	
	
protected:	
	GLuint					set_wnd_pixel_format(HDC hDC);
	GLuint					create_gl_context(HDC hDC);
	GLuint					destroy_gl_context();

	GLuint					buildshader(const char* pszsource, GLenum shaderType);
	GLuint					buildprogram(const char* vertexShaderSource, const char* fragmentShaderSource);
	
	void					renderframe();

	GLuint					opengl_init();
	GLuint					opengl_init_wnd(int nWndWidth, int nWndHeight);
	GLuint					opengl_func_event();

protected:
	static void				display();
	static void				timeFunc(int value);

	static void				keyboard(unsigned char key, int x , int y);
	static void				mouse(int button, int state, int x, int y);
	static void				mousemove(int x, int y);
	
protected:
	static int				m_nPosX;
	static int				m_nPosY;

	static int				m_nWndWidth;
	static int				m_nWndHeight;
	
private:
	char*					m_pszWndTitle;
	FILE*					m_file;	

	int						m_argc;
	char*					m_argv;
	
	GLuint					m_nProgramId;
	
	GLuint					m_nTextureUniformY;
	GLuint					m_nTextureUniformU;
	GLuint					m_nTextureUniformV;

	GLuint					m_nTextureIdY;
	GLuint					m_nTextureIdU;
	GLuint					m_nTextureIdV;

	GLuint					m_nPixelWidth;
	GLuint					m_nPixelHeight;

	HGLRC					m_hRC;
	
	unsigned char*			m_pBuffer;
	unsigned char*  		m_pPlane[3];
};

#endif