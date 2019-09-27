#ifndef __DATA_BUFFER_H__
#define __DATA_BUFFER_H__

class CDataBuffer
{
public:
	CDataBuffer();
	~CDataBuffer();
	
public:
	int		GetDataLength() const { return m_nDataLen; }
	char*	GetDataBuffer() const { return m_pDataBuffer; }
	
	bool	AllocDataBuffer(int nDataSize);
	void	FreeMemory();
	
private:
	int		m_nDataLen;
	char	*m_pDataBuffer;
};

#endif