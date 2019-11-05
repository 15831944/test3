	void			SetDevData(CDevData devData);
	CDevData&		GetDevData();

	void			SetApiHandle(void *pApiHandle);
	void*			GetApiHandle();

	void			SetDataQueue(IDataFrame *pDataFrame);
	IDataFrame*		GetDataQueue();
	
	void*			m_pApiHandle;

	CDevData		m_devData;
	CDataQueue<IDataFrame> m_dataQueue;
	

void CDevHandle::SetDevData(CDevData devData)
{
	m_devData = devData;
}

CDevData& CDevHandle::GetDevData()
{
	return m_devData;
}

void CDevHandle::SetApiHandle(void *pApiHandle)
{
	if (m_pApiHandle != NULL)
	{
		delete m_pApiHandle;
		m_pApiHandle = NULL;
	}

	if (pApiHandle != NULL)
	{
		m_pApiHandle = pApiHandle;
	}
}

void* CDevHandle::GetApiHandle()
{
	return m_pApiHandle;
}

void CDevHandle::SetDataQueue(IDataFrame *pDataFrame)
{
	if (pDataFrame == NULL)
	{
		return;
	}
}

IDataFrame*	CDevHandle::GetDataQueue()
{
	return NULL;
}


template <typename T>
class ObjectPtr
{
private:
	T* t;

public:
	ObjectPtr();
	~ObjectPtr() {}

public:
	operator bool ()
	{
		return t != NULL;
	}

	T* operator ->()
	{
		return t;
	}
};

template <typename T>
ObjectPtr<T>::ObjectPtr()
{
	t = NULL;
}