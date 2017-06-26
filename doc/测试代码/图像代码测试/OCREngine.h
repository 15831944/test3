#ifndef __OCRENGINE_H__
#define __OCRENGINE_H__

#include <allheaders.h>
#include <baseapi.h>
#include <resultiterator.h>
#include <strngs.h>

using namespace std;
using namespace tesseract;
class COcrEngine
{
public:
	COcrEngine();
	~COcrEngine();
	
public:
	const char*  		GetOcrEngineVersion();
	bool				Init(const char *datapath, const char *language);
	bool				Init(const char *datapath, const char *language, int ocrEngineMode);
	void    			Close();
	void	    		Clear();
	
	void				SetPageSegMode(int nPageMode);	
	void				SetImage(const Pix *pix);
	void				SetImage(const unsigned char *imagedata, int nWidth, int nHeight, int bytes_per_pixel, int bytes_per_line);
	void				SetRect(int nLeft, int nTop, int nWidth, int nHeight);
	bool				SetVariable(const char *name, const char *value);
	
	bool				GetBoolVariable(const char *name, bool *value);
	bool				GetIntVariable(const char *name,  int  *value);
	bool				GetDoubleVariable(const char *name, double *value);
	const char*			GetStringVariable(const char *name);
	
	string				Recognize(const char *filepath);
	const char*			GetText();

protected:
	TessBaseAPI			m_tessBaseAPI;	
};
#endif