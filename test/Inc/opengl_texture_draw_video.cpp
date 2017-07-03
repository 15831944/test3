#include "stdafx.h"
#include "opengl_texture_draw_video.h"

int opengl_texture_draw_video::m_nPosX = 100;
int opengl_texture_draw_video::m_nPosY = 100;

int opengl_texture_draw_video::m_nWndWidth  = 500;
int opengl_texture_draw_video::m_nWndHeight = 500;

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
	m_argc = 1;
	m_argv = "test.exe";
	
	m_pBuffer = new unsigned char[m_nPixelWidth*m_nPixelHeight*3/2];
	if (m_pBuffer == NULL)
	{
		exit(0);
	}
	memset(m_pBuffer, 0x0, m_nPixelWidth*m_nPixelHeight*3/2);
	
	m_pPlane[0] = m_pBuffer;
	m_pPlane[1] = m_pPlane[0] + m_nPixelWidth*m_nPixelHeight;
	m_pPlane[2] = m_pPlane[1] + m_nPixelWidth*m_nPixelHeight/4;
	
	m_file = NULL;
	m_pszWndTitle = "this is a test!";
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

void opengl_texture_draw_video::mouse(int button, int state, int x, int y)
{
}

void opengl_texture_draw_video::mousemove(int x, int y)
{
}

void opengl_texture_draw_video::keyboard(unsigned char key, int x , int y)
{
	switch(key)
	{
	case 27:
			exit(0);
		break;
	}
}

void opengl_texture_draw_video::display()
{
	opengl_texture_draw_video::Instance().renderframe();
}

void opengl_texture_draw_video::timeFunc(int value)
{
	display();
	glutTimerFunc(40, timeFunc, 1);
}

//////////////////////////////////////////////////////////////////////////
//
GLuint opengl_texture_draw_video::init_context(OPENGL_WND_TYPE hWndType)
{
	switch(hWndType)
	{
	case OPENGL_CONSOLE_TYPE:
		{
			if (!opengl_init())
			{
				return GL_FALSE;
			}
		}
		break;

	case OPENGL_WIN32_TYPE:
		{

		}
		break;

	default:
		break;
	}
}

GLuint opengl_texture_draw_video::opengl_init()
{
	GLint  nWndHandle = -1;	
	
	glutInit(&m_argc, &m_argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowPosition(m_nPosX,m_nPosY);
	glutInitWindowSize(m_nWndWidth,m_nWndHeight);
	
	nWndHandle = glutCreateWindow(m_pszWndTitle);
	if(nWndHandle < 1)
	{
		printf("glutCreateWindow failed!\n");
		return GL_FALSE;
	}
	
	return GL_TRUE;
}

GLuint opengl_texture_draw_video::opengl_init_wnd(int nWndWidth, int nWndHeight)
{
	glClearColor(0.0,0.0,0.0,0.0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,0.0,nWndWidth,nWndHeight,-1.0,1.0);
	
	return GL_TRUE;
}

GLuint opengl_texture_draw_video::opengl_func_event()
{
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemove);
	
	return GL_TRUE;
}

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

void opengl_texture_draw_video::renderframe()
{

}

GLuint opengl_texture_draw_video::init_context()
{
	m_nProgramId = buildprogram(VERTEX_SHADER, FRAG_SHADER);
	if(m_nProgramId == GL_FALSE)
	{
		return GL_FALSE;
	}
	
	glUseProgram(m_nProgramId);
	
	m_nTextureUniformY = glGetUniformLocation(m_nProgramId, "tex_y");
	m_nTextureUniformU = glGetUniformLocation(m_nProgramId, "tex_u");
	m_nTextureUniformV = glGetUniformLocation(m_nProgramId, "tex_v");
	
	return GL_TRUE;
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
		PFD_DRAW_TO_BITMAP|
		PFD_SUPPORT_GDI|
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
	m_hRC = wglCreateContext(hDC);
	if (m_hRC == NULL)
	{
		return GL_FALSE;
	}

	if (wglMakeCurrent(hDC, m_hRC) == FALSE)
	{
		return GL_FALSE;
	}

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST); 

	return GL_TRUE;
}

GLuint opengl_texture_draw_video::destroy_gl_context()
{
	m_hRC = ::wglGetCurrentContext();
	if (::wglMakeCurrent (0,0) == FALSE)
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

//////////////////////////////////////////////////////////////////////////
//
