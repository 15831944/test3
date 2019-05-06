#ifndef __VIDEO_SERVER_CONTROL_H__
#define __VIDEO_SERVER_CONTROL_H__

class CVideoServerControl
{
public:
	CVideoServerControl();
	~CVideoServerControl();
	
public:
	bool				IsOpenServer();
	
	bool				OpenServer();
	void				CloseServer();
	
public:	
	bool				EnumDev();	
};

#endif