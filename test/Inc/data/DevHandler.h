#ifndef __DEV_HANDLER_H__
#define __DEV_HANDLER_H__

//DevHandler
class CDevHandler
{
public:
	CDevHandler();	
	virtual ~CDevHandler();

	static CDevHandler&	Instance();
	
public:
	void*		getDevObj(unsigned int uiMsgId);
	void		regDevObj(unsigned int uiMsgId, void *pObjectPtr);

	void		removeDevObj(unsigned int uiMsgId);
	void		clearDevObj();

private:
	std::map<unsigned int, void*> m_mapData;
};

#endif