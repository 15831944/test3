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