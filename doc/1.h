void CTextView::OnTimer(UINT_PTR nIDEvent)
{
    m_nWidth += 5;  // 在构造函数中初始化为 0；

    CClientDC dc( this );
    TEXTMETRIC tm;
    dc.GetTextMetrics( &tm );
    CRect rect;
    rect.left = 0;
    rect.top = 200;
    rect.right = m_nWidth;
    rect.bottom = rect.top + tm.tmHeight;
    dc.SetTextColor( RGB(255, 0, 0) );            // 字体设置成红色
    CString str;
    str.LoadStringW( IDS_STRINGVC );
    dc.DrawText( str, rect, DT_LEFT);            // 效果实现的核心函数,用来控制显示的矩形大小来控制效果

    CSize sz = dc.GetTextExtent( str );
    if( m_nWidth > sz.cx )                        // 超过字体长度时候从头显示
    {
        m_nWidth = 0;
        dc.SetTextColor( RGB(0, 255, 0) );
        dc.TextOutW( 0, 200, str );
    }

    CView::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
//
std::set<CWnd*> g_setModalWindows;
int DoModalHelper(CDialog* pDialog)
{
    g_setModalWindows.insert(pDialog);
    int nResult = pDialog->DoModal();
    g_setModalWindows.erase(pDialog);
    return nResult;
}

int DoModalHelper(CPropertySheet* pSheet)
{
    g_setModalWindows.insert(pSheet);
    int nResult = pSheet->DoModal();
    g_setModalWindows.erase(pSheet);
    return nResult;
}

/////////////////////////////////////////////////////////////////////////////
//
template<typename _T>
class CExcludeCharacter
{
public:
	typedef _T* Iter;
	CExcludeCharacter(const _T* ptr,int num, _T sentinel)
	{
		m_iNum = num;
		m_sentinel = sentinel;
		m_pSpecialCharacters = ptr;
	}

	~CExcludeCharacter()
	{
		m_pSpecialCharacters = NULL;
	}

	BOOL operator()(_T ch)
	{
		 BOOL bExisted = FALSE;

		const _T* pch = NULL;
		pch = find(m_pSpecialCharacters,m_pSpecialCharacters + m_iNum -1,ch);
		if (*pch != m_sentinel)
		{
			bExisted = TRUE;
		}
		return bExisted;
	}

private:
	_T m_sentinel;
	int m_iNum;
	const _T* m_pSpecialCharacters;
};


#include <algorithm>
#include <string>
using   namespace   std;

void test()
{
	char szSpecialCharacter[] = {'//','/',':','*','?','/"','<','>','|','0'};
	char szSource[MAX_PATH];
	memset(szSource,0x00,sizeof(szSource));
	sprintf_s(szSource,MAX_PATH,"Thi<s i>s a sa//mple ab/out h>ow to delete a gro*up of spec:ial chara?cters fro|m a give/n stri<ng usin|g C++");
	size_t size = strlen(szSource);
	std::string vSource(szSource,szSource+size);

	std::string::iterator iterCharacter = find_if(vSource.begin(),vSource.end(),CExcludeCharacter<char>(szSpecialCharacter, sizeof(szSpecialCharacter)/sizeof(char),szSpecialCharacter[9]));
	while (iterCharacter != vSource.end())
	{
		vSource.erase(iterCharacter);
		iterCharacter = find_if(vSource.begin(),vSource.end(),CExcludeCharacter<char>(szSpecialCharacter, sizeof(szSpecialCharacter)/sizeof(char),szSpecialCharacter[9]));
	}
}

/////////////////////////////////////////////////////////////////////////////
//