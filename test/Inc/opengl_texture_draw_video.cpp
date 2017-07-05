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
	m_hRC = NULL;
	m_hDC = NULL;

	m_bExit = FALSE;
	m_bIsMaximized = FALSE;

	m_dwThreadID = 0;
	m_nBufferLen = 0;

	m_nWndWidth	= 0;
	m_nWndHeight = 0;

	m_nPixelWidth = 0;
	m_nPixelHeight = 0;

	m_rect.SetRectEmpty();
	m_oldWindow.SetRectEmpty();
	m_originalRect.SetRectEmpty();

	m_pBuffer = NULL;
	memset(&m_pPlane, 0x0, sizeof(m_pPlane));

	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndEvent   = CreateEvent(NULL, TRUE, FALSE, NULL);
}

opengl_texture_draw_video::~opengl_texture_draw_video()
{
}

opengl_texture_draw_video& opengl_texture_draw_video::Instance()
{
	static opengl_texture_draw_video inst;
	return inst;
}

DWORD opengl_texture_draw_video::DrawSceneThreadProc(LPVOID lpParam)
{
	opengl_texture_draw_video* pDrawSceneProc = (opengl_texture_draw_video*)lpParam;
	if (pDrawSceneProc != NULL)
	{
		pDrawSceneProc->DisplayVideo();
	}
	
	return 0;
}

BEGIN_MESSAGE_MAP(opengl_texture_draw_video, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void opengl_texture_draw_video::OnPaint()
{
	ValidateRect(NULL);
}

void opengl_texture_draw_video::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) 
		return;

	//
	glViewport(0, 0, cx, cy);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(35.0f, (float)cx / (float)cy, 0.01f, 2000.0f);
	glMatrixMode(GL_MODELVIEW);

	switch (nType)
	{
	case SIZE_MAXIMIZED:
		{
			GetWindowRect(m_rect);
			MoveWindow(6, 6, cx - 14, cy - 14);
			GetWindowRect(m_rect);
			m_oldWindow = m_rect;
		}
		break;

	case SIZE_RESTORED:
		{
			if (m_bIsMaximized)
			{
				GetWindowRect(m_rect);
				MoveWindow(m_oldWindow.left, m_oldWindow.top - 18, m_originalRect.Width() - 4, m_originalRect.Height() - 4);
				GetWindowRect(m_rect);
				m_oldWindow = m_rect;
			}
		}
		break;
	}
}

void opengl_texture_draw_video::OnDraw(CDC *pDC)
{
	return;
}

void opengl_texture_draw_video::DisplayVideo()
{
	if (initcontext() == GL_FALSE)
	{
		return;
	}
	
	while(WaitForSingleObject(m_hEndEvent, 500) != WAIT_OBJECT_0)
	{
		if (!m_bExit)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
			drawscene();
			SwapBuffers(m_hDC);
		}
		else
		{
			SetEvent(m_hEndEvent);
		}
	}

	ResetEvent(m_hStartEvent);
	destroy_gl_context();
}
//////////////////////////////////////////////////////////////////////////
//
BOOL opengl_texture_draw_video::CreateGLContext(CRect rect, CWnd* pParent)
{
	BOOL bRet = FALSE;
	CString strClassName;

	if (pParent == NULL)
	{
		return FALSE;
	}

	if (rect.IsRectEmpty())
	{
		return FALSE;
	}
	
	if(WaitForSingleObject(m_hStartEvent, 0) != WAIT_OBJECT_0)
	{
		SetEvent(m_hStartEvent);
		ResetEvent(m_hEndEvent);
		
		strClassName = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_OWNDC, NULL, (HBRUSH)GetStockObject(WHITE_BRUSH), NULL);
		if (!CreateEx(NULL, strClassName, _T("OpenGL with MFC/CDialog"), WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, rect, pParent, NULL))
		{
			ResetEvent(m_hStartEvent);
			return FALSE;
		}
		
		m_hThread = CreateThread(NULL, 0, DrawSceneThreadProc, (LPVOID)this, 0, &m_dwThreadID);
		if(m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
		{
			ResetEvent(m_hStartEvent);
			return FALSE;
		}

		bRet = TRUE;
		m_bExit = FALSE;
		
		m_oldWindow	   = rect;
		m_originalRect = rect;
		
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	return bRet;
}

GLuint opengl_texture_draw_video::setframedata(const unsigned char* pFrameData, unsigned long ulDataLen, unsigned long ulVideoWidth, unsigned long ulVideoHeight)
{
	if (pFrameData == NULL || ulDataLen == 0)
	{
		return GL_FALSE;
	}

	if (ulVideoWidth == 0 || ulVideoHeight == 0)
	{
		return GL_FALSE;
	}

	FRAME_DATA_BUFFER* pFrameBuffer = new FRAME_DATA_BUFFER;
	if (pFrameBuffer == NULL)
	{
		return GL_FALSE;
	}
	memset(pFrameBuffer, 0x0, sizeof(FRAME_DATA_BUFFER));

	pFrameBuffer->pFrameData = new char[ulDataLen];
	if (pFrameBuffer->pFrameData == NULL)
	{
		if (pFrameBuffer != NULL)
		{
			delete pFrameBuffer;
			pFrameBuffer = NULL;
		}
		return GL_FALSE;
	}
	memset(pFrameBuffer->pFrameData, 0x0, ulDataLen);

	pFrameBuffer->uFrameDataLen = ulDataLen;
	pFrameBuffer->uPixelWidth   = ulVideoWidth;
	pFrameBuffer->uPixelHeight  = ulVideoHeight;
	memcpy(pFrameBuffer->pFrameData, pFrameData, ulDataLen);

	m_threadFrameQueue.Push(pFrameBuffer);
	return GL_TRUE;
}

GLuint opengl_texture_draw_video::getframedata()
{
	GLuint uRet = GL_FALSE;

	unsigned long uPixelWidth  = 0;
	unsigned long uPixelHeight = 0;
	unsigned long ulDataLen    = 0;

	FRAME_DATA_BUFFER* pFrameBuffer = NULL;

	if (m_threadFrameQueue.IsEmpty())
	{
		return GL_FALSE;
	}

	pFrameBuffer = (FRAME_DATA_BUFFER*)m_threadFrameQueue.Front();
	if (pFrameBuffer == NULL)
	{
		return GL_FALSE;
	}

	if (pFrameBuffer->pFrameData == NULL || pFrameBuffer->uFrameDataLen == 0 || pFrameBuffer->uPixelWidth == 0 || pFrameBuffer->uPixelHeight == 0)
	{
		uRet = GL_FALSE;
		goto part1;
	}

	uPixelWidth  = pFrameBuffer->uPixelWidth;
	uPixelHeight = pFrameBuffer->uPixelHeight;

	if (uPixelWidth != m_nPixelWidth || uPixelHeight != m_nPixelHeight)
	{
		if (m_pBuffer != NULL)
		{
			delete[] m_pBuffer;
			m_pBuffer = NULL;
		}

		m_nBufferLen = uPixelWidth*uPixelHeight*3/2;
		m_pBuffer = new unsigned char[m_nBufferLen];
		if (m_pBuffer == NULL)
		{
			uRet = GL_FALSE;
			goto part1;
		}
		memset(m_pBuffer, 0x0, m_nBufferLen);

		m_pPlane[0] = m_pBuffer;
		m_pPlane[1] = m_pPlane[0] + uPixelWidth*uPixelHeight;
		m_pPlane[2] = m_pPlane[1] + uPixelWidth*uPixelHeight/4;

		m_nPixelWidth  = uPixelWidth;
		m_nPixelHeight = uPixelHeight;
	}

	if (m_pBuffer == NULL)
	{
		uRet = GL_FALSE;
		goto part1;
	}
	else
	{
		memset(m_pBuffer, 0x0, m_nBufferLen);
	}

	if (m_nBufferLen < pFrameBuffer->uFrameDataLen)
	{
		uRet = GL_FALSE;
		goto part1;
	}

	uRet = GL_TRUE;
	memcpy(m_pBuffer, pFrameBuffer->pFrameData, pFrameBuffer->uFrameDataLen);

part1:
	if (pFrameBuffer != NULL)
	{
		if (pFrameBuffer->pFrameData != NULL)
		{
			delete[] pFrameBuffer->pFrameData;
			pFrameBuffer->pFrameData = NULL;
		}

		delete pFrameBuffer;
		pFrameBuffer = NULL;
	}

	m_threadFrameQueue.Pop();
	return uRet;
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

GLuint opengl_texture_draw_video::createsurface()
{
	//顶点数组(物体表面坐标取值范围是-1到1，数组坐标：做下,右下,左下,右下)
#if TEXTURE_ROTATE
	static const GLfloat vertexVertices[] = {
		-1.0f, -0.5f,
		0.5f, -1.0f,
		-0.5f,  1.0f,
		1.0f,  0.5f,
	};    
#else
	static const GLfloat vertexVertices[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f,  1.0f,
	};    
#endif

	//像素,纹理数组(纹理坐标取值范围是0-1,坐标原点位于左下角,数组坐标:左下,右下,左下,右下, 如果先左下,图像会倒过来)
#if TEXTURE_HALF
	static const GLfloat textureVertices[] = {
		0.0f,  1.0f,
		0.5f,  1.0f,
		0.0f,  0.0f,
		0.5f,  0.0f,
	}; 
#else
	static const GLfloat textureVertices[] = {
		0.0f,  1.0f,
		1.0f,  1.0f,
		0.0f,  0.0f,
		1.0f,  0.0f,
	}; 
#endif

	//定义顶点数组
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, vertexVertices);
	//启用属性数组
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	//定义像素纹理数组
	glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, textureVertices);
	//启用属性数组
	glEnableVertexAttribArray(ATTRIB_TEXTURE);

	//初始化纹理
	glGenTextures(1, &m_nTextureIdY);
	//绑定纹理
	glBindTexture(GL_TEXTURE_2D, m_nTextureIdY);

	//设置此纹理的一些属性
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//3
	glGenTextures(1, &m_nTextureIdU);
	glBindTexture(GL_TEXTURE_2D, m_nTextureIdU);   
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//4
	glGenTextures(1, &m_nTextureIdV); 
	glBindTexture(GL_TEXTURE_2D, m_nTextureIdV);    
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return GL_TRUE;
}

GLuint opengl_texture_draw_video::set_wnd_pixel_format()
{
	int nPixelFormat = 0;

	if (m_hDC == NULL)
	{
		return GL_FALSE;
	}

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
		0,0,0,0,0,0,						// cRGB Color Bits and shift 
		0,									// cAlphaBits  
		0,									// cAlphaShift  
		0,									// cAccumBits  
		0,0,0,0,							// RGB Alpha Accum Bits 
		16,									// cDepthBits 
		0,									// cStencilBits 
		0,									// cAuxBuffers
		PFD_MAIN_PLANE,						// iLayerType 
		0,									// bReserved 
		0,0,0								// Layer, Visible, Damage Mask 
	};

	nPixelFormat = ChoosePixelFormat(m_hDC, &pixelDesc);
	if (nPixelFormat == 0)
	{
		nPixelFormat = 1;
		if (DescribePixelFormat(m_hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)
		{
			return GL_FALSE;
		}
	}

	if (SetPixelFormat(m_hDC, nPixelFormat, &pixelDesc) == FALSE)
	{
		return GL_FALSE;
	}

	return GL_TRUE;
}

GLuint opengl_texture_draw_video::create_gl_context()
{
	HGLRC hRC = NULL;
	int OpenGLVersion[2];
	GLenum GlewInitResult;

	if (m_hDC == NULL)
	{
		return GL_FALSE;
	}
	
	int attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
        0
    };
		
	hRC = wglCreateContext(m_hDC);
	if (hRC == NULL)
	{
		return GL_FALSE;
	}

	if (wglMakeCurrent(m_hDC, hRC) == FALSE)
	{
		return GL_FALSE;
	}

	glewExperimental = GL_TRUE;
	GlewInitResult = glewInit();
	if(GlewInitResult != GLEW_OK)
	{
		return GL_FALSE;
	}

#if 0
	if(wglewIsSupported("WGL_ARB_create_context") == 1)
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
#endif

	m_hRC = hRC;
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

GLuint opengl_texture_draw_video::initcontext()
{
	m_hDC = GetDC()->GetSafeHdc();
	if (m_hDC == NULL)
	{
		return GL_FALSE;
	}

	if(set_wnd_pixel_format() == GL_FALSE)
	{
		return GL_FALSE;
	}

	if(create_gl_context() == GL_FALSE)
	{
		return GL_FALSE;
	}

	m_nProgramId = buildprogram(VERTEX_SHADER, FRAG_SHADER);
	if(m_nProgramId == GL_FALSE)
	{
		return GL_FALSE;
	}

	glUseProgram(m_nProgramId);

	m_nTextureUniformY = glGetUniformLocation(m_nProgramId, "tex_y");
	m_nTextureUniformU = glGetUniformLocation(m_nProgramId, "tex_u");
	m_nTextureUniformV = glGetUniformLocation(m_nProgramId, "tex_v");

	if (!createsurface())
	{
		return GL_FALSE;
	}

	initscene();
	return GL_TRUE;
}

void opengl_texture_draw_video::initscene()
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

	glClearColor(0.5f, 0.6f, 0.9f, 0.0f);
//	glClearColor(0.0, 0.0, 1.0, 0.0);
}

void opengl_texture_draw_video::drawscene()
{
#if 0
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();  
	gluLookAt(0.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);  

	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();  
	gluPerspective(45.0, 1.0, 0.1, 500.0);  
	glMatrixMode(GL_MODELVIEW); 

	glColor3f(1.0, 0.0, 0.0);  
	glBegin(GL_TRIANGLES);  
		glVertex3f(0.0, 25.0, 0.0);  
		glVertex3f(-25.0, -25.0, 0.0);  
		glVertex3f(25.0, -25.0, 0.0);  
	glEnd(); 

	glFlush();
#elif 0
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(50.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

//	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	glBegin(GL_TRIANGLES);
		glColor4ub(200,0,180,255);
		glVertex3f(0,0,-100.0f); 
		glVertex3f(10.0f, 0, -100.0f); 
		glVertex3f(0, 10.0f, -100.0f);
	glEnd();
#elif 0
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

// 	gluPerspective(50.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
// 	glMatrixMode(GL_MODELVIEW);
// 	glLoadIdentity();

	const GLfloat factor = 0.1f;//让正弦函数比例放大 
	GLfloat x;  

	//画x,y坐标  
	glEnable(GL_LINE_STIPPLE);//启动虚线  
	glLineStipple(2,0x0F0F);//设置虚线样式  
	glLineWidth(2.0f);    //设置线宽  

	glBegin(GL_LINES);  
		glVertex2f(-1.0f,0.0f);  
		glVertex2f(1.0f,0.0f);  
		glVertex2f(0.0f,-1.0f);  
		glVertex2f(0.0f,1.0f);  
	glEnd();  

	//画正弦函数曲线  
	glDisable(GL_LINE_STIPPLE);//禁止虚线  
	glBegin(GL_LINE_STRIP);  
		for (x=-1.0f/factor;x<1.0f/factor;x+=0.01f)  
		{  
			glVertex2f(x*factor,sin(x)*factor);  
		}  
	glEnd(); 
#elif 0
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(50.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_QUADS);
		// Front Side
		glVertex3f( 1.0f,  1.0f, 1.0f);
		glVertex3f(-1.0f,  1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f( 1.0f, -1.0f, 1.0f);

		// Back Side
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);

		// Top Side
		glVertex3f( 1.0f, 1.0f,  1.0f);
		glVertex3f( 1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f,  1.0f);

		// Bottom Side
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);

		// Right Side
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);

		// Left Side
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();
#elif 1
	if (getframedata() == GL_FALSE)
	{
		return;
	}
	
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	//允许建立一个绑定到目标纹理的有名称纹理
	glBindTexture(GL_TEXTURE_2D, m_nTextureIdY);
	//根据指定参数,生成一个2D纹理(Texture),相似函数还有glTexImage1D、glTexImage3D
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_nPixelWidth, m_nPixelHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_pPlane[0]);	//GL_RED
	//设置纹理,安装签名设置的规则讲图像或者纹理贴上(参数和选择的活跃纹理单元对应GL_TEXTURE0)
	glUniform1i(m_nTextureUniformY, 0);    

	//U
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_nTextureIdU);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_nPixelWidth/2, m_nPixelHeight/2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_pPlane[1]);       
	glUniform1i(m_nTextureUniformU, 1);
	//V
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_nTextureIdV);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_nPixelWidth/2, m_nPixelHeight/2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_pPlane[2]);    
	glUniform1i(m_nTextureUniformV, 2);  

	//绘制
	glFlush();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//	glutSwapBuffers();
#endif
}