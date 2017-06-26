// UseListBox.cpp : 实现文件
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



// CUseListBox 消息处理程序



void CUseListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (GetCount() == 0)
	{
		return;
	}
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	//下面这句看不太懂，获取这个指针有什么用,将结构体与控件关联，懂了
	List_AppendData * pData =(List_AppendData *)GetItemDataPtr(lpDrawItemStruct->itemID);//获得当前控件某行的指针
	//itemID为某行ID或索引

	//获取列表数据
	CString str;
	GetText(lpDrawItemStruct->itemID,str);//获取当前行数据
	//如果是选中状态
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

	//设置按钮矩形边界
	pData->buttomRC.left = lpDrawItemStruct->rcItem.right-80;
	pData->buttomRC.top = lpDrawItemStruct->rcItem.top+15;
	pData->buttomRC.right = pData->buttomRC.left+74;	//按钮宽度
	pData->buttomRC.bottom = pData->buttomRC.top+26;

	CBitmap bitmap;
	bitmap.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP5));
	CDC t_dc;
	t_dc.CreateCompatibleDC(&dc);//创建兼容tc
	t_dc.SelectObject(bitmap);
	dc.TransparentBlt(pData->buttomRC.left,
		pData->buttomRC.top,
		pData->buttomRC.Width(),
		pData->buttomRC.Height(),
		&t_dc,
		0,	//源dc原点x
		0,	//源dc原点y
		pData->buttomRC.Width(),
		pData->buttomRC.Height(),
		RGB(255,0,255));
	t_dc.DeleteDC();
	bitmap.DeleteObject();
	//CBrush t_brush1;
	//t_brush1.CreateSolidBrush(RGB(0,245,0));
	//dc.FillRect(&lpDrawItemStruct->rcItem,&t_brush1);
	//画图标
	//原版画大图标会失真，把LoadIcon改成LoadImage后可解决，可以显示大于32*32的图标
	HICON t_hIcon;
	t_hIcon =(HICON)LoadImage(AfxGetInstanceHandle(),pData->hIcon,IMAGE_ICON,64,64,
		LR_CREATEDIBSECTION|LR_VGACOLOR |LR_LOADMAP3DCOLORS|LR_LOADTRANSPARENT|LR_VGACOLOR);	//pData->hIcon的内容？
	DrawIconEx(dc.m_hDC,5,lpDrawItemStruct->rcItem.top+5,t_hIcon,64, 64, 0, NULL, DI_NORMAL | DI_COMPAT);
	//第一排字体
	HFONT t_hFont= CreateFont(20,0,0,0,600,0,0,0,1,2,1,0,FF_MODERN,_T("宋体"));
	dc.SelectObject(t_hFont);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(46,134,208));
	dc.TextOut(80,lpDrawItemStruct->rcItem.top+18,str,str.GetLength());
	//第二排字体
	HFONT t_hFont1= CreateFont(20,0,0,0,400,0,0,0,1,2,1,0,FF_MODERN,_T("宋体"));
	dc.SelectObject(t_hFont1);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(245,140,140));
	str = pData->strAppend;
	dc.TextOut(80,lpDrawItemStruct->rcItem.top+37,str,str.GetLength());
	//底部边线
	CPen t_pen(PS_SOLID,1,RGB(0,218,223));
	dc.SelectObject(t_pen);
	dc.MoveTo(0,lpDrawItemStruct->rcItem.bottom-1);
	dc.LineTo(lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom-1);
	dc.Detach();
}

void CUseListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{

	// TODO:  添加您的代码以确定指定项的大小
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

	// TODO: 在此处添加消息处理程序代码
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CListBox::OnEraseBkgnd(pDC);
}

void CUseListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int szButton;

	BOOL bQutside = TRUE;
	int z = ItemFromPoint(point, bQutside); // 获取鼠标当前所在的列表项

	CRect rcItem;    // 表项相对于列表框的坐标
	GetItemRect(z, &rcItem);
	//
	CRect rcBtn;     // 表项上按钮相对于列表框的坐标
	rcBtn.left = rcItem.right-80;
	rcBtn.top = rcItem.top+15;
	rcBtn.right = rcBtn.left+74; //按钮宽度
	rcBtn.bottom = rcBtn.top+26;

	if (rcBtn.PtInRect(point))
	{ 
		szButton = IDB_BITMAP1;  // 暗
	}
	else
	{
		szButton = IDB_BITMAP2;  // 亮
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
		int szButton;

		BOOL bQutside = TRUE;
		int idx = ItemFromPoint(point, bQutside); // 获取鼠标当前所在的列表项

		CRect rcItem;    // 表项相对于列表框的坐标
		GetItemRect(idx, &rcItem);
		//
		CString szText;
		szText.Format(_T("你点击了第 %d 个按钮"),idx);
		CRect rcBtn;     // 表项上按钮相对于列表框的坐标
		rcBtn.left = rcItem.right-80;
		rcBtn.top = rcItem.top+15;
		rcBtn.right = rcBtn.left+74; //按钮宽度
		rcBtn.bottom = rcBtn.top+26;

		if (rcBtn.PtInRect(point))
		{
			szButton = IDB_BITMAP1;  // 暗
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
			MessageBox(_T("我是")+strName+_T(",很高兴认识你"),_T("提示"),MB_OK);
		}else
		{
			CListBox::OnLButtonDown(nFlags, point);
		}
		m_bDown = FALSE;
}
