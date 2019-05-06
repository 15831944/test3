#ifndef __LEAF_MEMDC_H__
#define __LEAF_MEMDC_H__

namespace Leaf
{
	namespace Drawing
	{
		class CMemDC : public CDC
		{
		public:
			CMemDC(CDC* pDC, const CRect* pRect=NULL);
			CMemDC(CDC* pDC, CRect rect = CRect(0,0,0,0), BOOL bCopyFirst = FALSE);
			~CMemDC();
			
		public:
			CMemDC* operator->()		{ return this; }
			operator CMemDC*()			{ return this; }

		protected:
			CBitmap		m_bitmap;
			CRect		m_rect;
		
		private:
			BOOL		m_bMemDC;
			
			CDC*		m_pDC;
			CBitmap*	m_oldBitmap;
		};
	}
}

#endif