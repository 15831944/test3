void CDlgTest3Wnd::OnBnClickedBtnTest3()
{
	unsigned char* pBlockBuf = NULL;
	unsigned long BLOCK_SIZE_IN = 0;
	unsigned long BLOCK_SIZE_OUT = 0;

	unsigned long ulFileLen = 0;
	unsigned long uBlockLen = 0;
	unsigned long ulRemainLen = 0;

	unsigned long ulPixelWidth = 0;
	unsigned long ulPixelHeight = 0;

	unsigned short uBitCount = 0;

	FILE* file = NULL;
	char* pfilename = NULL;

#if (OPENGL_DRAW_WND_VIDEO == 0)
	BITMAPFILEHEADER fileHeader = {0};
	BITMAPINFOHEADER bmHeader = {0};

	pfilename = _T("527_1120.bmp");

	file = fopen(pfilename, "rb+");
	if (file == NULL)
	{
		return;
	}

	//
	if (fread(&fileHeader,sizeof(BITMAPFILEHEADER),1,file) == 0)
	{
		if (file != NULL)
		{
			fclose(file);
			file = NULL;
		}
		return;
	}

	if (fileHeader.bfType != 0x4D42)
	{
		return;
	}

	//
	fseek(file, sizeof(BITMAPFILEHEADER), 0);
	if (fread(&bmHeader, sizeof(BITMAPINFOHEADER), 1, file) == 0)
	{
		if (file != NULL)
		{
			fclose(file);
			file = NULL;
		}
		return;
	}

	uBitCount = bmHeader.biBitCount;
	ulPixelWidth = bmHeader.biWidth;
	ulPixelHeight = bmHeader.biHeight;

	//
	BLOCK_SIZE_OUT = (ulPixelWidth * uBitCount/8 +3)/4*4*ulPixelHeight;
	if (BLOCK_SIZE_OUT == 0)
	{
		if (file != NULL)
		{
			fclose(file);
			file = NULL;
		}
		return;
	}

	pBlockBuf = new unsigned char[BLOCK_SIZE_OUT];
	if (pBlockBuf == NULL)
	{
		if (file != NULL)
		{
			fclose(file);
			file = NULL;
		}
		return;
	}
	memset(pBlockBuf, 0x0, BLOCK_SIZE_OUT);

	if (fread(pBlockBuf, 1, BLOCK_SIZE_OUT, file) == 0)
	{
		if (file != NULL)
		{
			fclose(file);
			file = NULL;
		}
		return;	
	}

	uBlockLen = BLOCK_SIZE_OUT;
	m_gdiDrawVideo.SetFrameData(pBlockBuf, uBlockLen, ulPixelWidth, ulPixelHeight);
#else
	ulPixelWidth = 320;
	ulPixelHeight = 180;

	//
	file = fopen("test_yuv420p_320x180.yuv", "rb+");
	if (file == NULL)
	{
		return;
	}

	//
	fseek(file, 0, SEEK_END);
	ulFileLen = ftell(file);
	if (ulFileLen == 0)
	{
		if(file)
		{
			fclose(file);
			file = NULL;
		}		
		return;
	}

	BLOCK_SIZE_IN = ulPixelWidth*ulPixelHeight*3/2;
	BLOCK_SIZE_OUT = BLOCK_SIZE_IN * 3;

	pBlockBuf = new unsigned char[BLOCK_SIZE_OUT];
	if(pBlockBuf == NULL)
	{
		if(file)
		{
			fclose(file);
			file = NULL;
		}	
		return;
	}
	memset(pBlockBuf, 0x0, BLOCK_SIZE_OUT);

	ulRemainLen = ulFileLen;
	fseek(file, 0, SEEK_SET);

	while(ulRemainLen > 0)
	{
		if(ulRemainLen >= BLOCK_SIZE_IN)
		{
			uBlockLen    = BLOCK_SIZE_IN;
			ulRemainLen -= BLOCK_SIZE_IN;
		}
		else
		{
			uBlockLen    = ulRemainLen;
			ulRemainLen -= ulRemainLen;
		}

		memset(pBlockBuf, 0x0, BLOCK_SIZE_OUT);

		fread(pBlockBuf, 1, uBlockLen, file);
		m_openglDrawVideo.SetFrameData(pBlockBuf, uBlockLen, ulPixelWidth, ulPixelHeight);

		Sleep(50);
	}
#endif

	if(file)
	{
		fclose(file);
		file = NULL;
	}

	if (pBlockBuf)
	{
		delete[] pBlockBuf;
		pBlockBuf = NULL;
	}
}