#include "stdafx.h"
#include "OCREngine.h"

#ifdef _DEBUG
	#pragma comment(lib, "liblept168.lib")
	#pragma comment(lib, "libtesseract302.lib")
#else
	#if 0
 		#pragma comment(lib, "libtiff394.lib")
 		#pragma comment(lib, "libpng143.lib")
 		#pragma comment(lib, "giflib416.lib")
		#pragma comment(lib, "libjpeg8c.lib")
		#pragma comment(lib, "liblept168-static.lib")
		#pragma comment(lib, "libtesseract302-static.lib")
	#else
		#pragma comment(lib, "liblept168.lib")
		#pragma comment(lib, "libtesseract302.lib")
	#endif
#endif


COcrEngine::COcrEngine()
{
}

COcrEngine::~COcrEngine()
{
	Clear();
	Close();
}

//////////////////////////////////////////////////////////////////////
//
const char* COcrEngine::GetOcrEngineVersion()
{
	return m_tessBaseAPI.Version();
}


bool COcrEngine::Init(const char *datapath, const char *language)
{
	return Init(datapath, language, 3);
}


bool COcrEngine::Init(const char* datapath, const char* language, int ocrEngineMode)
{
	bool bRet = false;
	
	if (m_tessBaseAPI.Init(datapath, language, (OcrEngineMode)ocrEngineMode) == 0)
	{
		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}


void COcrEngine::Close()
{
	return m_tessBaseAPI.End();
}


void COcrEngine::Clear()
{
	return m_tessBaseAPI.Clear();
}


void COcrEngine::SetPageSegMode(int nPageMode)
{
//	return m_tessBaseAPI.SetPageSegMode(nPageMode);
}


void COcrEngine::SetImage(const Pix* pix)
{
	return m_tessBaseAPI.SetImage(pix);
}


void COcrEngine::SetImage(const unsigned char* imagedata, int nWidth, int nHeight, int bytes_per_pixel, int bytes_per_line)
{
	return m_tessBaseAPI.SetImage(imagedata, nWidth, nHeight, bytes_per_pixel, bytes_per_line);
}


void COcrEngine::SetRect(int nLeft, int nTop, int nWidth, int nHeight)
{
	return m_tessBaseAPI.SetRectangle(nLeft, nTop, nWidth, nHeight);
}


//
//tessedit_char_whitelist; tessedit_char_blacklist; classify_bln_numeric_mode;
bool COcrEngine::SetVariable(const char *name, const char *value)
{
	return m_tessBaseAPI.SetVariable(name, value);;
}


bool COcrEngine::GetBoolVariable(const char *name, bool *value)
{
	bool bRet = false, val = false;

	bRet = m_tessBaseAPI.GetBoolVariable(name, &val);
	if(bRet)
	{
		*value = val;
	}

	return bRet;
}


bool COcrEngine::GetIntVariable(const char *name,  int *value)
{
	bool bRet = false;
	int  val  = 0;

	bRet = m_tessBaseAPI.GetIntVariable(name, &val);
	if(bRet)
	{
		*value = val;
	}

	return bRet;
}


bool COcrEngine::GetDoubleVariable(const char *name, double *value)
{
	bool bRet = false;
	double val = 0.0;

	bRet = m_tessBaseAPI.GetDoubleVariable(name, &val);
	if(bRet)
	{
		*value = val;
	}

	return bRet;
}


const char* COcrEngine::GetStringVariable(const char *name)
{
	return m_tessBaseAPI.GetStringVariable(name);
}


string COcrEngine::Recognize(const char *filepath)
{
	STRING text_out;
	m_tessBaseAPI.ProcessPages(filepath, NULL, 0, &text_out);

	return text_out.string();
}


const char* COcrEngine::GetText()
{
	return (m_tessBaseAPI.GetUTF8Text());
}
