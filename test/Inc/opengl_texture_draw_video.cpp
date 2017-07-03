#include "stdafx.h"
#include "opengl_texture_draw_video.h"

const GLchar* VERTEX_SHADER = 
{
	"attribute vec4 vertexIn;	\n" 
	"attribute vec2 textureIn;	\n"
	"varying vec2 textureOut;	\n"
	"void main(void)			\n"
	"{							\n"
	"gl_Position = vertexIn;	\n" 
	"textureOut = textureIn;	\n"
	"}							\n"
};

const GLchar* FRAG_SHADER = 
{
	"uniform sampler2D tex_y;	\n" 
	"uniform sampler2D tex_u;	\n" 
	"uniform sampler2D tex_v;	\n" 
	"varying vec2 textureOut;	\n"
	"void main(void)			\n"
	"{							\n"
	"  mediump vec3 yuv;		\n" 
	"  lowp vec3 rgb;			\n"    
	"  yuv.x = texture2D(tex_y, textureOut).r;			\n" 
	"  yuv.y = texture2D(tex_u, textureOut).r - 0.5;	\n" 
	"  yuv.z = texture2D(tex_v, textureOut).r - 0.5;	\n" 
	"  rgb = mat3( 1,		  1,		1,				\n" 
	"			   0,		 -0.39465,  2.03211,		\n" 
	"			   1.13983,  -0.58060,  0) * yuv;		\n"  
	"  gl_FragColor = vec4(rgb, 1);						\n" 
	"}							\n"
};

opengl_texture_draw_video::opengl_texture_draw_video()
{
	m_nPixelWidth = 320;
	m_nPixelHeight = 188;
	m_pBuffer = new unsigned char[m_nPixelWidth*m_nPixelHeight*3/2];
	if (m_pBuffer == NULL)
	{
		exit(0);
	}
	memset(m_pBuffer, 0x0, m_nPixelWidth*m_nPixelHeight*3/2);
	
	m_pPlane[0] = m_pBuffer;
	m_pPlane[1] = m_pPlane[0] + m_nPixelWidth*m_nPixelHeight;
	m_pPlane[2] = m_pPlane[1] + m_nPixelWidth*m_nPixelHeight/4;
}

opengl_texture_draw_video::~opengl_texture_draw_video()
{
	if (m_pBuffer != NULL)
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
}

opengl_texture_draw_video& opengl_texture_draw_video::Instance()
{
	static opengl_texture_draw_video inst;
	return inst;
}

//////////////////////////////////////////////////////////////////////////
//
GLuint opengl_texture_draw_video::init_context(HDC hDC)
{
	if(set_wnd_pixel_format(hDC) == GL_FALSE)
	{
		return GL_FALSE;
	}
	
	if(create_gl_context(hDC) == GL_FALSE)
	{
		return GL_FALSE;
	}
	
	InitScene();
	
/*	
	m_nProgramId = buildprogram(VERTEX_SHADER, FRAG_SHADER);
	if(m_nProgramId == GL_FALSE)
	{
		return GL_FALSE;
	}
	
	glUseProgram(m_nProgramId);
	
	m_nTextureUniformY = glGetUniformLocation(m_nProgramId, "tex_y");
	m_nTextureUniformU = glGetUniformLocation(m_nProgramId, "tex_u");
	m_nTextureUniformV = glGetUniformLocation(m_nProgramId, "tex_v");
*/	
	return GL_TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
GLuint opengl_texture_draw_video::buildshader(const char* pszsource, GLenum shaderType)
{
	GLint nCompiled = 0;
	GLint nInfoLen = 0;

	char* buf = NULL;

	GLuint shaderHandle = glCreateShader(shaderType);
	if (shaderHandle == 0)
	{
		return GL_FALSE;
	}
	
	glShaderSource(shaderHandle, 1, &pszsource, NULL);
	glCompileShader(shaderHandle);

	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &nCompiled); 
	if (nCompiled == GL_FALSE)
	{
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &nInfoLen);
		if (nInfoLen == GL_TRUE)
		{
			buf = new char[nInfoLen+1];
			if (buf != NULL)
			{
				glGetShaderInfoLog(shaderHandle, nInfoLen, NULL, buf);   
				
				delete buf;
				buf = NULL;
			}
			glDeleteShader(shaderHandle);  
			shaderHandle = 0;  
		}
	}
	
	return shaderHandle;
}

GLuint opengl_texture_draw_video::buildprogram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	GLint linkStatus = GL_FALSE;
	GLint nInfoLen = 0;

	char* buf = NULL;

	GLuint vertexShader = buildshader(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint fragmentShader = buildshader(fragmentShaderSource, GL_FRAGMENT_SHADER); 

	GLuint programHandle = glCreateProgram(); 
	if (programHandle == GL_FALSE)
	{
		return GL_FALSE;
	}
	
	glAttachShader(programHandle, vertexShader);  
	glAttachShader(programHandle, fragmentShader); 

	glBindAttribLocation(programHandle, ATTRIB_VERTEX, "vertexIn");
	glBindAttribLocation(programHandle, ATTRIB_TEXTURE, "textureIn");

	glLinkProgram(programHandle);

	glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE)
	{
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &nInfoLen);
		if (nInfoLen == GL_TRUE)
		{
			buf = new char[nInfoLen+1];
			if (buf != NULL)
			{
				glGetProgramInfoLog(programHandle, nInfoLen, NULL, buf); 

				delete buf;
				buf = NULL;
			}
			
			glDeleteProgram(programHandle);  
			programHandle = 0;  
		}
	}
	
	return programHandle;
}


GLuint opengl_texture_draw_video::set_wnd_pixel_format(HDC hDC)
{
	int nPixelFormat = 0;

	static PIXELFORMATDESCRIPTOR pixelDesc=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,									// nVersion 
		PFD_DRAW_TO_WINDOW|					// dwFlags
		PFD_SUPPORT_OPENGL|
		PFD_DOUBLEBUFFER|
		PFD_STEREO_DONTCARE,
		PFD_TYPE_RGBA,						// iPixelType 
		24,									// cColorBits
		8,16,8,8,8,0,						// cRGB Color Bits and shift 
		0,									// cAlphaBits  
		0,									// cAlphaShift  
		64,									// cAccumBits  
		16,16,16,0,							// RGB Alpha Accum Bits 
		32,									// cDepthBits 
		8,									// cStencilBits 
		0,									// cAuxBuffers
		PFD_MAIN_PLANE,						// iLayerType 
		0,									// bReserved 
		0,0,0								// Layer, Visible, Damage Mask 
	};

	nPixelFormat = ChoosePixelFormat(hDC, &pixelDesc);
	if (nPixelFormat == 0)
	{
		nPixelFormat = 1;
		if (DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)
		{
			return GL_FALSE;
		}
	}

	if (SetPixelFormat(hDC, nPixelFormat, &pixelDesc) == FALSE)
	{
		return GL_FALSE;
	}

	return GL_TRUE;
}

GLuint opengl_texture_draw_video::create_gl_context(HDC hDC)
{
	HGLRC hRC = NULL;
	int OpenGLVersion[2];
	GLenum GlewInitResult;
	
	int attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_FLAGS_ARB, 0,
        0
    };
		
	hRC = wglCreateContext(hDC);
	if (hRC == NULL)
	{
		return GL_FALSE;
	}

	if (wglMakeCurrent(hDC, hRC) == FALSE)
	{
		return GL_FALSE;
	}

	glewExperimental = GL_TRUE;
	GlewInitResult = glewInit();
	if(GlewInitResult != GLEW_OK)
	{
		return GL_FALSE;
	}
	
	if(wglewIsSupported("WGL_ARB_create_context"))
	{
		m_hRC = wglCreateContextAttribsARB(hDC, 0, attribs);
		wglMakeCurrent(NULL,NULL);
		
		wglDeleteContext(hRC);
		wglMakeCurrent(hDC, m_hRC);
	}
	else
	{
		m_hRC = hRC;
	}
	
	const GLubyte *GLVersionString = glGetString(GL_VERSION);
	
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
	
	if(m_hRC == NULL)
	{
		return GL_FALSE;
	}

	return GL_TRUE;
}

GLuint opengl_texture_draw_video::destroy_gl_context()
{
	m_hRC = ::wglGetCurrentContext();
	if (::wglMakeCurrent (NULL, NULL) == FALSE)
	{
		return GL_FALSE;
	}

	if (m_hRC)
	{
		if(::wglDeleteContext(m_hRC)==FALSE)
		{
			return GL_FALSE;
		}
	}

	return GL_TRUE;
}

void opengl_texture_draw_video::InitScene()
{
	//启用阴影平滑
	glShadeModel(GL_SMOOTH);
	//黑色背景
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	//设置深度缓存
	glClearDepth(1.0);  
	//启用深度测试  
	glEnable(GL_DEPTH_TEST);
	//所作深度测试的类型 
	glDepthFunc(GL_LEQUAL);
	//告诉系统对透视进行修正
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D); 
}

void opengl_texture_draw_video::drawScene(HDC hDC)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
    glLoadIdentity(); 
    glTranslatef(0.0f  ,0.0f  ,-5.0f ); 

	SwapBuffers(hDC);
}
