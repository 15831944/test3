#include "stdafx.h"
#include "CryptAlg_test.h"

#include <des.h>

void cbc3_test(char key1[8], char key2[8], char key3[8], char iv[8], char input[24], char ouput[24])
{
	unsigned char k1[8] = {0};
	unsigned char k2[8] = {0};
	unsigned char k3[8] = {0};

	unsigned char indata[24]  = {0};
	unsigned char outdata[24] = {0};
	unsigned char outEncdata[24] = {0};
	unsigned char outDecdata[24] = {0};

	DES_key_schedule s1 = {0};
	DES_key_schedule s2 = {0};
	DES_key_schedule s3 = {0};
	
	DES_cblock ivdata = {0};
	DES_cblock ivec_copy = {0};

	memcpy(k1, key1, 8);
	memcpy(k2, key2, 8);
	memcpy(k3, key3, 8);

	memcpy(ivdata, iv, 8);
	memcpy(indata, input, 24);
	memcpy(outdata,ouput, 24);

	DES_set_odd_parity(&k1);
	DES_set_odd_parity(&k2);
	DES_set_odd_parity(&k3);

	DES_set_key_unchecked(&k1, &s1);
	DES_set_key_unchecked(&k2, &s2);
	DES_set_key_unchecked(&k3, &s3);

	memcpy(&ivec_copy, &ivdata, sizeof(ivec_copy));
	DES_ede3_cbc_encrypt(indata, outEncdata, 24, &s1, &s2, &s3, &ivec_copy, DES_ENCRYPT);
	if (memcmp(outEncdata, outdata, sizeof(outdata)) != 0)
	{
		AfxMessageBox("º”√‹ ß∞‹!");
		return;
	}

	memcpy(&ivec_copy, &ivdata, sizeof(ivec_copy));
	DES_ede3_cbc_encrypt(outEncdata, outDecdata, 24, &s1, &s2, &s3, &ivec_copy, DES_DECRYPT);
	if (memcmp(indata, outDecdata, sizeof(outDecdata)) != 0)
	{
		AfxMessageBox("Ω‚√‹ ß∞‹!");
		return;
	}
}



void cbc3_test(char key1[8], char key2[8], char key3[8], char iv[8], char input[24], char ouput[24])
{
	unsigned char k1[8] = {0};
	unsigned char k2[8] = {0};
	unsigned char k3[8] = {0};

	unsigned char indata[24]  = {0};
	unsigned char outdata[24] = {0};
	unsigned char outEncdata[24] = {0};
	unsigned char outDecdata[24] = {0};

	DES_key_schedule s1 = {0};
	DES_key_schedule s2 = {0};
	DES_key_schedule s3 = {0};
	
	DES_cblock ivdata = {0};
	DES_cblock ivec_copy = {0};

	memcpy(k1, key1, 8);
	memcpy(k2, key2, 8);
	memcpy(k3, key3, 8);

	memcpy(ivdata, iv, 8);
	memcpy(indata, input, 24);
	memcpy(outdata,ouput, 24);

	DES_set_odd_parity(&k1);
	DES_set_odd_parity(&k2);
	DES_set_odd_parity(&k3);

	DES_set_key_unchecked(&k1, &s1);
	DES_set_key_unchecked(&k2, &s2);
	DES_set_key_unchecked(&k3, &s3);

	memcpy(&ivec_copy, &ivdata, sizeof(ivec_copy));
	DES_ede3_cbc_encrypt(indata, outEncdata, 24, &s1, &s2, &s3, &ivec_copy, DES_ENCRYPT);
	if (memcmp(outEncdata, outdata, sizeof(outdata)) != 0)
	{
		AfxMessageBox("º”√‹ ß∞‹!");
		return;
	}

	memcpy(&ivec_copy, &ivdata, sizeof(ivec_copy));
	DES_ede3_cbc_encrypt(outEncdata, outDecdata, 24, &s1, &s2, &s3, &ivec_copy, DES_DECRYPT);
	if (memcmp(indata, outDecdata, sizeof(outDecdata)) != 0)
	{
		AfxMessageBox("Ω‚√‹ ß∞‹!");
		return;
	}
}

void CryptTest1()
{
/*	{
		char* s11 = "\x61\xcb\x8c\xb0\x32\x2a\xc2\x5d";
		char* s12 = "\x98\xe3\x49\xc1\x0d\xb5\x67\xce";
		char* s13 = "\xf2\x43\x10\x61\x85\x6b\xa7\x15";

		char* iv11 = "\x65\xf5\x8f\x1a\x2b\x33\xf2\xb5";
		char* in11 = "\x8c\x06\xe0\x60\x68\x25\x9c\x95\x81\x46\xda\x41\x9d\xa8\x9c\x49\x2f\xee\x33\x35\x95\x11\xbd\xa0";
		char* out11 = "\x93\x27\xed\xc7\x35\xb9\xe5\x3c\x7b\x10\x3e\x39\x01\x41\x61\x04\xe7\xf2\xd9\x63\x96\xca\x57\xf1";

		cbc3_test(s11, s12, s13, iv11, in11, out11);
	}*/
	
/*	{
		char* s21 = "\x15\x61\x6b\x76\xae\x0e\x98\x01";
		char* s22 = "\x76\xce\x9d\x94\xa7\xe3\x73\xa4";
		char* s23 = "\x19\xd9\x15\x98\x9b\xba\x83\x40";

		char* iv21 = "\x60\xef\xc2\xc6\xa2\x40\x01\xc7"; 
		char* in21 = "\x8b\x4d\xf4\x37\xad\x1c\xc2\x4e\xcc\xc4\x4b\x17\x67\xf7\xfa\xec\xf8\x94\x6f\x7a\x84\x56\x81\x09";
		char* out21 = "\x68\xdf\x82\xcb\xd9\xcd\x3d\xca\x12\x0e\x2e\x39\xba\xf7\x5a\x8c\x41\xbd\x6f\x9d\x85\xfe\x1b\x1d";

		cbc3_test(s21, s22, s23, iv21, in21, out21);
	}*/
	
	{
		char* s31 = "\xd5\x2a\x4f\xa4\x13\x9e\x73\x15";
		char* s32 = "\x6d\x75\xa8\x15\x07\xd3\x7c\x79";
		char* s33 = "\xd5\xe0\xa7\x91\xf8\xf2\x9d\xcd";

		char* iv31 = "\x4c\xdb\x56\xb8\x6f\x0e\x2a\x59";
		char* in31 = "\xbe\x64\x20\x24\x7d\x2b\x6b\xf4\xd9\xc0\xa0\x9b\x8d\x88\x6e\x50\x6f\xf8\xb6\x4a\x7e\x52\x52\x93";
		char* out31 = "\x01\x83\x75\x7b\xd6\x03\xff\xd8\xe9\x6d\x6c\x92\x24\x25\x35\xfa\x43\x4c\x40\xff\xec\xb0\x8b\x50";

		cbc3_test(s31, s32, s33, iv31, in31, out31);
	}	
}

void CryptTest2()
{
	unsigned char uKey[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

	unsigned char k1[8] = {0};
	unsigned char k2[8] = {0};
	unsigned char k3[8] = {0};

	unsigned char indata[8]  = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
	unsigned char outdata[24] = {0};
	unsigned char outEncdata[24] = {0};
	unsigned char outDecdata[24] = {0};

	DES_key_schedule s1 = {0};
	DES_key_schedule s2 = {0};
	DES_key_schedule s3 = {0};

	DES_cblock ivdata = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	DES_cblock ivec_copy = {0};

	memcpy(k1, uKey, 8);
	memcpy(k2, uKey+8, 8);
	memcpy(k3, uKey, 8);

	DES_set_odd_parity(&k1);
	DES_set_odd_parity(&k2);
	DES_set_odd_parity(&k3);

	DES_set_key_unchecked(&k1, &s1);
	DES_set_key_unchecked(&k2, &s2);
	DES_set_key_unchecked(&k3, &s3);

	memcpy(&ivec_copy, &ivdata, sizeof(ivec_copy));
	DES_ede3_cbc_encrypt(indata, outEncdata, 8, &s1, &s2, &s3, &ivec_copy, DES_ENCRYPT);

	memcpy(&ivec_copy, &ivdata, sizeof(ivec_copy));
	DES_ede3_cbc_encrypt(outEncdata, outDecdata, 8, &s1, &s2, &s3, &ivec_copy, DES_DECRYPT);	
}