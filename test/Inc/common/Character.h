#ifndef __CHARACTER_H__
#define __CHARACTER_H__

//https://www.cnblogs.com/chinxi/p/6129774.html
//https://blog.csdn.net/imxiangzi/article/details/77370160
//https://blog.csdn.net/xjz729827161/article/details/53064653

class CControl
{
public:
	CControl();
	virtual ~CControl();

public:
	static int		IsStringANSI(const char* pszString);
	static int		IsStringUtf8(const char* pszString);
	static int		IsStringGBK(const char* pszString);
	static int		IsStringUnicode(const char* pszString);
};

class CConvert  
{
public:
	CConvert();
	virtual ~CConvert();
			
public:
	static int  	UnicodeToANSI(char*    szDesc,  const wchar_t* szSrc);       	//wchar_t --> char
    static int   	ANSIToUnicode(wchar_t* szDesc,  const char*    szSrc);        	//char    --> wchar_t 
			
	static int		UTF8ToUnicode(wchar_t* szDesc,  const char*    szSrc);			//UTF-8	  --> UCS2	
	static int		UnicodeToUTF8(char*    szDesc,  const wchar_t* szSrc);			//UCS2	  --> UTF-8
			
	static int		UTF8ToANSI(char* szDesc, const char* szSrc);
	static int		ANSIToUTF8(char* szDesc, const char* szSrc);
			
	static int		StringToHexString(char* szDesc, const char* szSrc, int nLen, char chTag=0);
	static int      HexStringToBytes(unsigned char*	 szDesc, const char* szSrc,int nLen);			
	static int		BytesHexToString(char*  szDesc, const unsigned char* szSrc,int nLen);	
};

#endif