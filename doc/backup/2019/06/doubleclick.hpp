
void CTestView::OnButton1()
{
    // must be static due to re-entrant function
    static bool double_wait = false;
    
    // must be static due to re-entrant function
    static bool handled = false ;
    
    MSG    msg ;
    if (double_wait)
        {
        // this is a double click
        // do double click code here
        TRACE("Double click\n") ;
        handled = true ;
        return ;
        }
    else
        handled = false ;
        double_wait = true ;

    UINT    time = GetDoubleClickTime() ;
    UINT    slice = time / 10 ;
    
    // break up the OS double click time into some small
    // chunks to avoid hogging the CPU this could be done
    // in many different ways, but I chose this one
    for (int i = 0 ; i < 10 && !handled ; i++)
    {
        // break up the sleep into chuncks to give better response
        if (i == 9)
            Sleep(slice + time % 10) ;
        else
        {
            // dispatch any messages that have come in,
            // including any clicks on this button again.
            Sleep(slice) ;
        }
        while (::PeekMessage(&msg, NULL, 0, 0,
                                PM_NOYIELD | PM_REMOVE))
        {
            ::TranslateMessage(&msg) ;
            ::DispatchMessage(&msg) ;
        }
    }
    // time ran out,
    if (!handled)
    {
        // this is a single click
        // do single click code here
        TRACE("Single click\n") ;
    }
    double_wait = false ;
}