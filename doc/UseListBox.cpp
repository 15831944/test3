// UseListBox.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DrawListBox.h"
#include "UseListBox.h"


// CUseListBox

IMPLEMENT_DYNAMIC(CUseListBox, CListBox)

CUseListBox::CUseListBox():m_bDown(FALSE)
{

}

CUseListBox::~CUseListBox()
{
}


BEGIN_MESSAGE_MAP(CUseListBox, CListBox)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CUseListBox ��Ϣ�������



void CUseListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (GetCount() == 0)
	{
		return;
	}
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	//������俴��̫������ȡ���ָ����ʲô��,���ṹ����ؼ�����������
	List_AppendData * pData =(List_AppendData *)GetItemDataPtr(lpDrawItemStruct->itemID);//��õ�ǰ�ؼ�ĳ�е�ָ��
	//itemIDΪĳ��ID������

	//��ȡ�б�����
	CString str;
	GetText(lpDrawItemStruct->itemID,str);//��ȡ��ǰ������
	//�����ѡ��״̬
	if (lpDrawItemStruct->itemAction | ODA_SELECT && lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		CBrush t_brush1;
		t_brush1.CreateSolidBrush(RGB(0,239,244));
		dc.FillRect(&lpDrawItemStruct->rcItem,&t_brush1);
	}
	else
	{
  		CBrush t_brush1;
 		t_brush1.CreateSolidBrush(RGB(255,255,255));
 		dc.FillRect(&lpDrawItemStruct->rcItem,&t_brush1);

// 		CBrush t_brush1;
// 		t_brush1.CreateSolidBrush(RGB(0,245,255));
// 		dc.FillRect(LPRECT(pData->buttomRC),&t_brush1);
	}

	//���ð�ť���α߽�
	pData->buttomRC.left = lpDrawItemStruct->rcItem.right-80;
	pData->buttomRC.top = lpDrawItemStruct->rcItem.top+15;
	pData->buttomRC.right = pData->buttomRC.left+74;	//��ť���
	pData->buttomRC.bottom = pData->buttomRC.top+26;

	CBitmap bitmap;
	bitmap.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP5));
	CDC t_dc;
	t_dc.CreateCompatibleDC(&dc);//��������tc
	t_dc.SelectObject(bitmap);
	dc.TransparentBlt(pData->buttomRC.left,
		pData->buttomRC.top,
		pData->buttomRC.Width(),
		pData->buttomRC.Height(),
		&t_dc,
		0,	//Դdcԭ��x
		0,	//Դdcԭ��y
		pData->buttomRC.Width(),
		pData->buttomRC.Height(),
		RGB(255,0,255));
	t_dc.DeleteDC();
	bitmap.DeleteObject();
	//CBrush t_brush1;
	//t_brush1.CreateSolidBrush(RGB(0,245,0));
	//dc.FillRect(&lpDrawItemStruct->rcItem,&t_brush1);
	//��ͼ��
	//ԭ�滭��ͼ���ʧ�棬��LoadIcon�ĳ�LoadImage��ɽ����������ʾ����32*32��ͼ��
	HICON t_hIcon;
	t_hIcon =(HICON)LoadImage(AfxGetInstanceHandle(),pData->hIcon,IMAGE_ICON,64,64,
		LR_CREATEDIBSECTION|LR_VGACOLOR |LR_LOADMAP3DCOLORS|LR_LOADTRANSPARENT|LR_VGACOLOR);	//pData->hIcon�����ݣ�
	DrawIconEx(dc.m_hDC,5,lpDrawItemStruct->rcItem.top+5,t_hIcon,64, 64, 0, NULL, DI_NORMAL | DI_COMPAT);
	//��һ������
	HFONT t_hFont= CreateFont(20,0,0,0,600,0,0,0,1,2,1,0,FF_MODERN,_T("����"));
	dc.SelectObject(t_hFont);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(46,134,208));
	dc.TextOut(80,lpDrawItemStruct->rcItem.top+18,str,str.GetLength());
	//�ڶ�������
	HFONT t_hFont1= CreateFont(20,0,0,0,400,0,0,0,1,2,1,0,FF_MODERN,_T("����"));
	dc.SelectObject(t_hFont1);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(245,140,140));
	str = pData->strAppend;
	dc.TextOut(80,lpDrawItemStruct->rcItem.top+37,str,str.GetLength());
	//�ײ�����
	CPen t_pen(PS_SOLID,1,RGB(0,218,223));
	dc.SelectObject(t_pen);
	dc.MoveTo(0,lpDrawItemStruct->rcItem.bottom-1);
	dc.LineTo(lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom-1);
	dc.Detach();
}

void CUseListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{

	// TODO:  ������Ĵ�����ȷ��ָ����Ĵ�С
	lpMeasureItemStruct->itemHeight = 75;
}
void CUseListBox::InsertStr(CString str,int iIndex,CString strAppend,UINT uIcon,BOOL state)
{

	List_AppendData * pData = new List_AppendData;
	pData->hIcon = MAKEINTRESOURCE(uIcon);
	pData->strAppend = strAppend;
	pData->bState = state;
	int i = SetItemDataPtr(InsertString(iIndex,str),pData);
}
void CUseListBox::OnDestroy()
{
	CListBox::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	int iCount = GetCount();
	for (int i=0; i<iCount; i++)
	{
		List_AppendData * pData = (List_AppendData *)GetItemDataPtr(i);
		delete pData;
		pData = NULL;
	}
}

BOOL CUseListBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CListBox::OnEraseBkgnd(pDC);
}

void CUseListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int szButton;

	BOOL bQutside = TRUE;
	int z = ItemFromPoint(point, bQutside); // ��ȡ��굱ǰ���ڵ��б���

	CRect rcItem;    // ����������б�������
	GetItemRect(z, &rcItem);
	//
	CRect rcBtn;     // �����ϰ�ť������б�������
	rcBtn.left = rcItem.right-80;
	rcBtn.top = rcItem.top+15;
	rcBtn.right = rcBtn.left+74; //��ť���
	rcBtn.bottom = rcBtn.top+26;

	if (rcBtn.PtInRect(point))
	{ 
		szButton = IDB_BITMAP1;  // ��
	}
	else
	{
		szButton = IDB_BITMAP2;  // ��
	}
	CDC dc;
	dc.Attach(GetDC()->m_hDC);

	CBitmap bitmap;
	bitmap.LoadBitmap(MAKEINTRESOURCE(szButton));
	CDC t_dc;
	t_dc.CreateCompatibleDC(&dc);
	t_dc.SelectObject(bitmap);
	dc.TransparentBlt(rcBtn.left,
		rcBtn.top,
		rcBtn.Width(),
		rcBtn.Height(),
		&t_dc,
		0,
		0,
		rcBtn.Width(),
		rcBtn.Height(),
		RGB(255,0,255));
	t_dc.DeleteDC();
	bitmap.DeleteObject();
	m_bDown = TRUE;

	CListBox::OnMouseMove(nFlags, point);
}

void CUseListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		int szButton;

		BOOL bQutside = TRUE;
		int idx = ItemFromPoint(point, bQutside); // ��ȡ��굱ǰ���ڵ��б���

		CRect rcItem;    // ����������б�������
		GetItemRect(idx, &rcItem);
		//
		CString szText;
		szText.Format(_T("�����˵� %d ����ť"),idx);
		CRect rcBtn;     // �����ϰ�ť������б�������
		rcBtn.left = rcItem.right-80;
		rcBtn.top = rcItem.top+15;
		rcBtn.right = rcBtn.left+74; //��ť���
		rcBtn.bottom = rcBtn.top+26;

		if (rcBtn.PtInRect(point))
		{
			szButton = IDB_BITMAP1;  // ��
			CDC dc;
			dc.Attach(GetDC()->m_hDC);

			CBitmap bitmap;
			bitmap.LoadBitmap(MAKEINTRESOURCE(szButton));
			CDC t_dc;
			t_dc.CreateCompatibleDC(&dc);
			t_dc.SelectObject(bitmap);
			dc.TransparentBlt(rcBtn.left,
				rcBtn.top,
				rcBtn.Width(),
				rcBtn.Height(),
				&t_dc,
				0,
				0,
				rcBtn.Width(),
				rcBtn.Height(),
				RGB(255,0,255));
			t_dc.DeleteDC();
			bitmap.DeleteObject();


			CString strName=_T("");
			GetText(idx,strName);
			MessageBox(_T("����")+strName+_T(",�ܸ�����ʶ��"),_T("��ʾ"),MB_OK);
		}else
		{
			CListBox::OnLButtonDown(nFlags, point);
		}
		m_bDown = FALSE;
}
