#include "stdafx.h"
#include "opengl_wnd_draw_video.h"

static const GLchar* VERTEX_SHADER = 
{
	"attribute vec4 vertexIn;	\n" 
	"attribute vec2 textureIn;	\n"
	"varying vec2 textureOut;	\n"
	"void main(void)			\n"
	"{							\n"
	"  gl_Position = vertexIn;	\n" 
	"  textureOut = textureIn;	\n"
	"}							\n"
};

static const GLchar* FRAG_SHADER = 
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

opengl_wnd_draw_video::opengl_wnd_draw_video()
{
	m_hRC = NULL;
	m_hDC = NULL;
	m_hWnd = NULL;

	m_bExit = FALSE;

	m_nProcTimeOver = 80;
	m_nCloseTimeOver = 500;

	m_dwThreadID = 0;
	m_nBufferLen = 0;

	m_nWndWidth	= 0;
	m_nWndHeight = 0;

	m_nPixelWidth = 0;
	m_nPixelHeight = 0;

	m_rect.SetRectEmpty();

	m_pBuffer = NULL;
	memset(&m_pPlane, 0x0, sizeof(m_pPlane));

	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndEvent   = CreateEvent(NULL, TRUE, FALSE, NULL);
}

opengl_wnd_draw_video::~opengl_wnd_draw_video()
{
	if (!CloseGLProc())
	{
		return;
	}
}

//////////////////////////////////////////////////////////////////////////
//
opengl_wnd_draw_video& opengl_wnd_draw_video::Instance()
{
	static opengl_wnd_draw_video inst;
	return inst;
}

DWORD opengl_wnd_draw_video::DrawSceneThreadProc(LPVOID lpParam)
{
	opengl_wnd_draw_video* pDrawSceneProc = (opengl_wnd_draw_video*)lpParam;
	if (pDrawSceneProc != NULL)
	{
		pDrawSceneProc->displayvideo();
	}
	
	return 0;
}

void opengl_wnd_draw_video::displayvideo()
{
	if (m_hWnd == NULL)
	{
		return;
	}

	if (m_hDC == NULL)
	{
		return;
	}
	
	if (initcontext() == GL_FALSE)
	{
		return;
	}
	
	while(WaitForSingleObject(m_hEndEvent, m_nProcTimeOver) != WAIT_OBJECT_0)
	{
		if (!m_bExit)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);		//GL_STENCIL_BUFFER_BIT

//			glMatrixMode(GL_PROJECTION);
//			glLoadIdentity();

 			if (drawscene() == GL_FALSE)
 			{
				m_nProcTimeOver = 50;
 				continue;
 			}

			SwapBuffers(m_hDC);
		}
		else
		{
			SetEvent(m_hEndEvent);
		}
	}

	destroy_gl_context();
	ResetEvent(m_hStartEvent);
}

static inline unsigned char CONVERT_ADJUST(double tmp)  
{
	return (unsigned char)((tmp >= 0 && tmp <= 255)?tmp:(tmp < 0 ? 0 : 255));  
}

GLuint opengl_wnd_draw_video::CONVERT_YUV420PtoRGB24(unsigned char* yuv_src,unsigned char* rgb_dst,int nWidth, int nHeight)
{
	int ypSize = 0;
	int upSize = 0;
	int offSet = 0;

	int rgb_width = 0;
	int u_width = 0;

	unsigned char Y,U,V,R,G,B;

	unsigned char* y_planar = NULL;
	unsigned char* u_planar = NULL;
	unsigned char* v_planar = NULL;

	if (yuv_src == NULL || rgb_dst == NULL)
	{
		return GL_FALSE;
	}

	if (nWidth == 0 || nHeight == 0)
	{
		return GL_FALSE;
	}

	rgb_width = nWidth * 3; 
	u_width = (nWidth >> 1);

	ypSize = nWidth * nHeight;
	upSize = (ypSize>>2);
	offSet = 0;  

	y_planar = yuv_src;  
	u_planar = yuv_src + ypSize;  
	v_planar = u_planar + upSize;

	for(int i = 0; i < nHeight; i++) 
	{
		for(int j = 0; j < nWidth; j ++)  
		{
			// Get the Y value from the y planar  
			Y = *(y_planar + nWidth * i + j); 

			// Get the V value from the u planar  
			offSet = (i>>1) * (u_width) + (j>>1);

			V = *(u_planar + offSet);  
			U = *(v_planar + offSet);  

			
			// Cacular the R,G,B values  
			// Method 1  
			R = CONVERT_ADJUST((Y + (1.4075 * (V - 128))));  
			G = CONVERT_ADJUST((Y - (0.3455 * (U - 128) - 0.7169 * (V - 128))));  
			B = CONVERT_ADJUST((Y + (1.7790 * (U - 128))));  

			/* 
            // The following formulas are from MicroSoft' MSDN 
            int C,D,E; 
            // Method 2 
            C = Y - 16; 
            D = U - 128; 
            E = V - 128; 
            R = CONVERT_ADJUST(( 298 * C + 409 * E + 128) >> 8); 
            G = CONVERT_ADJUST(( 298 * C - 100 * D - 208 * E + 128) >> 8); 
            B = CONVERT_ADJUST(( 298 * C + 516 * D + 128) >> 8); 
            R = ((R - 128) * .6 + 128 )>255?255:(R - 128) * .6 + 128;  
            G = ((G - 128) * .6 + 128 )>255?255:(G - 128) * .6 + 128;  
            B = ((B - 128) * .6 + 128 )>255?255:(B - 128) * .6 + 128;  
            */ 

			offSet = rgb_width * i + j * 3; 

			rgb_dst[offSet] = B;  
			rgb_dst[offSet + 1] = G;  
			rgb_dst[offSet + 2] = R; 
		}
	}

	return GL_TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL opengl_wnd_draw_video::CreateGLContext(FRAME_DATA_TYPE hDataType, CRect rect, HWND hWnd)
{
	BOOL bRet = FALSE;
	HDC hDC = NULL;

	if (rect.IsRectEmpty())
	{
		return FALSE;
	}
	
	if(hWnd == NULL)
	{
		return FALSE;
	}

	hDC = ::GetDC(hWnd);
	if (hDC == NULL)
	{
		return FALSE;
	}

	//WS_CLIPSIBLINGS : 创建父窗口使用的Windows风格，用于重绘时裁剪子窗口所覆盖的区域;
	//WS_CLIPCHILDREN : 创建子窗口使用的Windows风格，用于重绘时剪裁其他子窗口所覆盖的区域;
	
	if(WaitForSingleObject(m_hStartEvent, 0) != WAIT_OBJECT_0)
	{
		SetEvent(m_hStartEvent);
		ResetEvent(m_hEndEvent);
		
		m_hDC = hDC;
		m_hWnd = hWnd;
		
		m_rect = rect;
		m_hDataType = hDataType;

		::SetWindowLong(hWnd, GWL_STYLE, ::GetWindowLong(hWnd, GWL_STYLE) | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CS_HREDRAW | CS_VREDRAW | CS_OWNDC);

		m_hThread = CreateThread(NULL, 0, DrawSceneThreadProc, (LPVOID)this, 0, &m_dwThreadID);
		if(m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
		{
			ResetEvent(m_hStartEvent);
			return FALSE;
		}

		bRet = TRUE;
		m_bExit = FALSE;
		
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	return bRet;
}

BOOL opengl_wnd_draw_video::CloseGLProc()
{
	m_bExit = TRUE;

	WaitForSingleObject(m_hEndEvent, m_nCloseTimeOver);

// 	if (m_nTextureIdY != 0)
// 	{
// 		glDeleteTextures(1, &m_nTextureIdY);
// 	}
// 
// 	if (m_nTextureIdU != 0)
// 	{
// 		glDeleteTextures(1, &m_nTextureIdU);
// 	}
// 
// 	if (m_nTextureIdV != 0)
// 	{
// 		glDeleteTextures(1, &m_nTextureIdV);
// 	}

	if (destroy_gl_context() == GL_FALSE)
	{
		return FALSE;
	}

	if (m_hDC != NULL)
	{
		::ReleaseDC(m_hWnd, m_hDC);
		m_hDC = NULL;
	}

	while(!m_threadFrameQueue.IsEmpty())
	{
		FRAME_DATA_BUFFER* pFrameBuffer = (FRAME_DATA_BUFFER*)m_threadFrameQueue.Front();
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
	}

	return TRUE;
}

void opengl_wnd_draw_video::SetProcTimeOver(GLuint nProcTime, GLuint nCloseTime)
{
	m_nProcTimeOver = nProcTime;
	m_nCloseTimeOver = nCloseTime;
}

GLuint opengl_wnd_draw_video::SetFrameData(const unsigned char* pFrameData, unsigned long ulDataLen, unsigned long ulVideoWidth, unsigned long ulVideoHeight)
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

	pFrameBuffer->pFrameData = new unsigned char[ulDataLen];
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

GLuint opengl_wnd_draw_video::getframedata()
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

		if (m_hDataType == FRAME_YUV420PDRAW_TYPE)
		{
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
		}
		else if (m_hDataType == FRAME_YUV420PTORGB24_TYPE)
		{
			m_nBufferLen = uPixelWidth*uPixelHeight*3;
			m_pBuffer = new unsigned char[m_nBufferLen];
			if (m_pBuffer == NULL)
			{
				uRet = GL_FALSE;
				goto part1;
			}
			memset(m_pBuffer, 0x0, m_nBufferLen);
		}
		else
		{
			uRet = GL_FALSE;
			goto part1;
		}

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

	if (m_hDataType == FRAME_YUV420PDRAW_TYPE)
	{
		uRet = GL_TRUE;
		memcpy(m_pBuffer, pFrameBuffer->pFrameData, pFrameBuffer->uFrameDataLen);
	}
	else if (m_hDataType == FRAME_YUV420PTORGB24_TYPE)
	{
		if (CONVERT_YUV420PtoRGB24(pFrameBuffer->pFrameData, m_pBuffer, uPixelWidth, uPixelHeight) == GL_FALSE)
		{
			uRet = GL_FALSE;
		}

		uRet = GL_TRUE;
	}

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
GLuint opengl_wnd_draw_video::buildshader(const char* pszsource, GLenum shaderType)
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

GLuint opengl_wnd_draw_video::buildprogram(const char* vertexShaderSource, const char* fragmentShaderSource)
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
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return programHandle;
}

GLuint opengl_wnd_draw_video::createsurface()
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

GLuint opengl_wnd_draw_video::set_wnd_pixel_format()
{
	int nPixelFormat = 0;

	if (m_hWnd == NULL)
	{
		return GL_FALSE;
	}

	if (m_hDC == NULL)
	{
		return GL_FALSE;
	}

	static PIXELFORMATDESCRIPTOR pixelDesc=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,									// nVersion 
		PFD_DRAW_TO_WINDOW |				// dwFlags
		PFD_SUPPORT_OPENGL |	
		PFD_DOUBLEBUFFER,
//		PFD_DRAW_TO_BITMAP |
//		PFD_SUPPORT_GDI	   | 
//		PFD_STEREO_DONTCARE,
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

GLuint opengl_wnd_draw_video::create_gl_context()
{
	HGLRC hRC = NULL;
	int OpenGLVersion[2];

	GLenum GlewInitResult;

	if (m_hWnd == NULL)
	{
		return GL_FALSE;
	}

	if (m_hDC == NULL)
	{
		return GL_FALSE;
	}
	
	const int iPixelFormatAttributeList[] = 
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,                        // 绘制到窗口
		WGL_SUPPORT_OPENGL_ARB,GL_TRUE,							// 支持OpenGL
		WGL_ACCELERATION_ARB ,WGL_FULL_ACCELERATION_ARB,		// 硬件加速
		WGL_DOUBLE_BUFFER_ARB,GL_TRUE,							// 双缓冲
		WGL_PIXEL_TYPE_ARB,WGL_TYPE_RGBA_ARB,					// RGBA
		WGL_COLOR_BITS_ARB,32,									// 颜色位数32
		WGL_DEPTH_BITS_ARB,24,									// 深度位数24
		WGL_STENCIL_BITS_ARB,8,									// 模板位数8
		WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,				// 双缓冲swap方式直接交换
		WGL_SAMPLES_ARB, 4,										// 4倍抗锯齿
		0
	};

	const int iContextAttributeList[] =
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
	wglChoosePixelFormatARB(m_hDC, iPixelFormatAttributeList, NULL, 1, &iPixelFormat,(UINT *)&iNumFormat);
	if (!SetPixelFormat(m_hDC,iPixelFormat,&pfd))
	{
		return GL_FALSE;
	}

	hRC = wglCreateContextAttribsARB(m_hDC,NULL,iContextAttributeList);
	if (hRC == NULL)
	{
		::wglMakeCurrent(m_hDC,hRC);
		return GL_FALSE;
	}

	if(wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		m_hRC = wglCreateContextAttribsARB(m_hDC, 0, attribs);
		wglMakeCurrent(NULL,NULL);

		wglDeleteContext(hRC);
		wglMakeCurrent(m_hDC, m_hRC);
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

GLuint opengl_wnd_draw_video::destroy_gl_context()
{
	m_hRC = ::wglGetCurrentContext();
	if (m_hRC != NULL)
	{
		if (::wglMakeCurrent(NULL, NULL) == FALSE)
		{
			return GL_FALSE;
		}

		if(::wglDeleteContext(m_hRC) == FALSE)
		{
			return GL_FALSE;
		}
	}

	return GL_TRUE;
}

GLuint opengl_wnd_draw_video::initcontext()
{
	if(set_wnd_pixel_format() == GL_FALSE)
	{
		return GL_FALSE;
	}

	if(create_gl_context() == GL_FALSE)
	{
		return GL_FALSE;
	}

	if(m_hDataType == FRAME_YUV420PDRAW_TYPE)
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

		if (!createsurface())
		{
			return GL_FALSE;
		}
	}
	else if(m_hDataType == FRAME_YUV420PTORGB24_TYPE)
	{
	}
	else
	{
		return GL_FALSE;
	}

	initscene();
	return GL_TRUE;
}

void opengl_wnd_draw_video::initscene()
{
	//启用阴影平滑
	glShadeModel(GL_SMOOTH);
	//黑色背景
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
	//设置深度缓存
	glClearDepth(1.0f);
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

GLuint opengl_wnd_draw_video::drawscene()
{
#if 0
	wglMakeCurrent(NULL, NULL);
	wglMakeCurrent(m_hDC, m_hRC);

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
	wglMakeCurrent(NULL, NULL);
#elif 0
	wglMakeCurrent(NULL, NULL);
	wglMakeCurrent(m_hDC, m_hRC);

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

	glFlush();
	wglMakeCurrent(NULL, NULL);
#elif 0
	wglMakeCurrent(NULL, NULL);
	wglMakeCurrent(m_hDC, m_hRC);

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

	glFlush();
	wglMakeCurrent(NULL, NULL);
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
	if (m_hWnd == NULL)
	{
		return GL_FALSE;
	}

	if (m_hDC == NULL)
	{
		return GL_FALSE;
	}
	
	if (getframedata() == GL_FALSE)
	{
		return GL_FALSE;
	}
	
	wglMakeCurrent(NULL, NULL);
	wglMakeCurrent(m_hDC, m_hRC);

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if(m_hDataType == FRAME_YUV420PDRAW_TYPE)
	{
		//Y
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
	}
	else if(m_hDataType == FRAME_YUV420PTORGB24_TYPE)
	{
		glRasterPos3f(-1.0f,1.0f,0);
		glPixelZoom((float)m_rect.Width()/(float)m_nPixelWidth, -(float)m_rect.Height()/(float)m_nPixelHeight);
		glDrawPixels(m_nPixelWidth, m_nPixelHeight, GL_RGB, GL_UNSIGNED_BYTE, m_pBuffer);
	}
	 
	//绘制
	glFlush();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	wglMakeCurrent(NULL, NULL);
#endif
	return GL_TRUE;
}