void CTextView::OnTimer(UINT_PTR nIDEvent)
{
    m_nWidth += 5;  // �ڹ��캯���г�ʼ��Ϊ 0��

    CClientDC dc( this );
    TEXTMETRIC tm;
    dc.GetTextMetrics( &tm );
    CRect rect;
    rect.left = 0;
    rect.top = 200;
    rect.right = m_nWidth;
    rect.bottom = rect.top + tm.tmHeight;
    dc.SetTextColor( RGB(255, 0, 0) );            // �������óɺ�ɫ
    CString str;
    str.LoadStringW( IDS_STRINGVC );
    dc.DrawText( str, rect, DT_LEFT);            // Ч��ʵ�ֵĺ��ĺ���,����������ʾ�ľ��δ�С������Ч��

    CSize sz = dc.GetTextExtent( str );
    if( m_nWidth > sz.cx )                        // �������峤��ʱ���ͷ��ʾ
    {
        m_nWidth = 0;
        dc.SetTextColor( RGB(0, 255, 0) );
        dc.TextOutW( 0, 200, str );
    }

    CView::OnTimer(nIDEvent);
}