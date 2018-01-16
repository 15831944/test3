#ifndef __MEMDC_H__
#define __MEMDC_H__

class CMemDC : public CDC
{
public:
	CMemDC(CDC* pDC, const CRect* pRect=NULL) 
		: CDC(),
		m_oldBitmap(NULL),
		m_pDC(pDC)
	{
		ASSERT(m_pDC != NULL);
		m_bMemDC = !pDC->IsPrinting();						//确定正在使用的设备上下文是否用于打印

		if (pRect == NULL)
		{
			pDC->GetClipBox(&m_rect);						//获取当前剪切边界周围最紧绑定矩形的维数
		}
		else
		{
			m_rect = *pRect;
		}

		if (m_bMemDC)
		{
			CreateCompatibleDC(pDC);
			pDC->LPtoDP(&m_rect);							//逻辑单位转换为设备单位	

			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());		
			m_oldBitmap = SelectObject(&m_bitmap);

			SetMapMode(pDC->GetMapMode());					//设置当前映射模式
			
			SetWindowExt(pDC->GetWindowExt());				//设置对应窗口的X和Y坐标
			SetViewportExt(pDC->GetViewportExt());			//设置视区X和Y范围

			pDC->DPtoLP(&m_rect);							//设备单位转换为逻辑单位
			SetWindowOrg(m_rect.left, m_rect.top);			//设置设备上下文的窗口起点 
		}
		else
		{
			m_bPrinting = pDC->m_bPrinting;
			m_hDC = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
		
		FillSolidRect(m_rect, pDC->GetBkColor());
	}

	~CMemDC()
	{
		if (m_bMemDC)
		{
			//StretchBlt
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),  
                    this, m_rect.left, m_rect.top, SRCCOPY);
			SelectObject(m_oldBitmap);
		}
		else
		{
			m_hDC = m_hAttribDC = NULL;
		}
	}

	CMemDC* operator->()	{return this;} 
	operator CMemDC*()		{return this;}

private:
	CBitmap		m_bitmap;
	CBitmap*	m_oldBitmap;
	CDC*		m_pDC;
	CRect		m_rect;
	BOOL		m_bMemDC;
};
#endif