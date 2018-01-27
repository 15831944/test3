#ifndef __MEMDCEX_H__
#define __MEMDCEX_H__

class CMemDCEx : public CDC
{
public:
	CMemDCEx(CDC* pDC, const CRect* pRect=NULL) : CDC(), m_oldBitmap(NULL),m_pDC(pDC)
	{
		ASSERT(m_pDC != NULL);
		m_bMemDC = !pDC->IsPrinting();						//ȷ������ʹ�õ��豸�������Ƿ����ڴ�ӡ

		if (pRect == NULL)
		{
			pDC->GetClipBox(&m_rect);						//��ȡ��ǰ���б߽���Χ����󶨾��ε�ά��
		}
		else
		{
			m_rect = *pRect;
		}

		if (m_bMemDC)
		{
			CreateCompatibleDC(pDC);
			pDC->LPtoDP(&m_rect);							//�߼���λת��Ϊ�豸��λ	

			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());		
			m_oldBitmap = SelectObject(&m_bitmap);

			SetMapMode(pDC->GetMapMode());					//���õ�ǰӳ��ģʽ
			
			SetWindowExt(pDC->GetWindowExt());				//���ö�Ӧ���ڵ�X��Y����
			SetViewportExt(pDC->GetViewportExt());			//��������X��Y��Χ

			pDC->DPtoLP(&m_rect);							//�豸��λת��Ϊ�߼���λ
			SetWindowOrg(m_rect.left, m_rect.top);			//�����豸�����ĵĴ������ 
		}
		else
		{
			m_bPrinting = pDC->m_bPrinting;
			m_hDC = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
		
		FillSolidRect(m_rect, pDC->GetBkColor());
	}

	CMemDCEx(CDC* pDC, CRect rect = CRect(0,0,0,0), BOOL bCopyFirst = FALSE) : CDC(), m_oldBitmap(NULL), m_pDC(pDC)
	{
		ASSERT(m_pDC != NULL);								//���Բ�����Ϊ��
		m_bMemDC = !pDC->IsPrinting();

		if (m_bMemDC)
		{
			//����һ���ڴ�DC
			CreateCompatibleDC(pDC);
			if ( rect == CRect(0,0,0,0) )
			{
				pDC->GetClipBox(&m_rect);
			}
			else
			{
				m_rect = rect;
			}
			//��������λͼ
			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			//������λͼ����
			m_oldBitmap = SelectObject(&m_bitmap);
			//�ƶ�����ԭ��
			SetWindowOrg(m_rect.left, m_rect.top);
			//���θ���
			if(bCopyFirst)
			{
				this->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),m_pDC, m_rect.left, m_rect.top, SRCCOPY);
			}
		} 
		else
		{
			//����һ������ǰDC�����йصĸ���
			m_bPrinting = pDC->m_bPrinting;
			m_hDC = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
	}

	~CMemDCEx()
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

	CMemDCEx* operator->()		{return this;} 
	operator CMemDCEx*()		{return this;}

private:
	CBitmap		m_bitmap;
	CBitmap*	m_oldBitmap;
	CDC*		m_pDC;
	CRect		m_rect;
	BOOL		m_bMemDC;
};
#endif