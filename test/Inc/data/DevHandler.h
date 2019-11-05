#ifndef __DEV_HANDLER_H__
#define __DEV_HANDLER_H__

#include "./ObjectPtr.h"

//DevHandler
class CDevHandler
{
public:
	CDevHandler();	
	virtual ~CDevHandler();

	static CDevHandler&	Instance();
	
public:
	void			regDevObj();
	CObjectPtr*		getDevObj(unsigned int uiMsgId);

	void			clearDevObj();

protected:
	void			insertObjectHandler(unsigned int uiMsgId, CObjectPtr *pObjectPtr);

private:
	std::map<unsigned int, CObjectPtr> m_mapData;
};

#endif