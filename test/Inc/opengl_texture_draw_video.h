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

class opengl_texture_draw_video
{
public:
	opengl_texture_draw_video();
	~opengl_texture_draw_video();
	
public:
	static opengl_texture_draw_video& Instance();

public:
	GLuint					init_context(HDC hDC);
	void					drawScene(HDC hDC);
	
protected:	
	GLuint					buildshader(const char* pszsource, GLenum shaderType);
	GLuint					buildprogram(const char* vertexShaderSource, const char* fragmentShaderSource);
	
	GLuint					set_wnd_pixel_format(HDC hDC);
	GLuint					create_gl_context(HDC hDC);
	GLuint					destroy_gl_context();
	
	void					InitScene();
	
	
private:
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
	HDC						m_hDC;
	
	unsigned char*			m_pBuffer;
	unsigned char*  		m_pPlane[3];
};

#endif