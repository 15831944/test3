#include "stdafx.h"
#include "zlibClass.h"


#if (ZLIB_MACROS == 1)
int Compress(const char * DestName,const char *SrcName)
{
	FILE * fp_in = NULL;int len = 0;char buf[16384];

	int re = 0;

	if( NULL == (fp_in = fopen(SrcName,"rb")))
	{
		return -1;
	}

	/////////////////////////////////////////////
	gzFile out = gzopen(DestName,"wb6f");

	if(out == NULL)
	{
		return -1;
	}

	for(;;)
	{
		len = fread(buf,1,sizeof(buf),fp_in);

		if(ferror(fp_in))
		{
			re = -1;
			break;
		}

		if(len == 0) break;

		if(gzwrite(out, buf, (unsigned)len) != len)
		{
			re = -1;
		}
	}

	gzclose(out);

	fclose(fp_in);

	return re;
}

int UnCompress(const char * DestName,const char *SrcName)
{
	FILE * fp_out = NULL;int re = 0;

	gzFile in;int len = 0;char buf[16384];

	in = gzopen(SrcName,"rb");

	if(in == NULL)
	{
		return -1;
	}

	if(NULL == (fp_out = fopen(DestName,"wb")))
	{
		gzclose(in);
		return -1;
	}

	for (;;)
	{
		len = gzread(in,buf,sizeof(buf));

		if(len < 0)
		{
			re = -1;
			break;
		}

		if(len == 0) break;

		if(fwrite(buf,1,(unsigned)len,fp_out)!=len)
		{
			re = -1;
			break;
		}
	}

	fclose(fp_out);
	gzclose(in);

	return re;
}
#endif
