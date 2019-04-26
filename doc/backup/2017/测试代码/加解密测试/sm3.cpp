#include "stdafx.h"
#include "sm3.h"

static const unsigned sm3_iv[] = {0x7380166f, 0x4914b2b9, 0x172442d7, 0xda8a0600, 0xa96f30bc, 0x163138aa, 0xe38dee4d, 0xb0fb0e4e};

unsigned ROTATE(unsigned x, unsigned n)
{
	return (x << n) | (x >> (32 - n));
}

unsigned P0(unsigned X)
{
	return ((X) ^ ROTATE((X), 9) ^ ROTATE((X), 17));
}

unsigned P1(unsigned X)
{
	return ((X) ^ ROTATE((X), 15) ^ ROTATE((X), 23));
}

unsigned FF_00_15(unsigned X, unsigned Y, unsigned Z)
{
	return (X ^ Y ^ Z);
}

unsigned FF_16_63(unsigned X, unsigned Y, unsigned Z)
{
	return ((X & Y) | (X & Z) | (Y & Z));
}

unsigned GG_00_15(unsigned X, unsigned Y, unsigned Z)
{
	return (X ^ Y ^ Z);
}

unsigned GG_16_63(unsigned X, unsigned Y, unsigned Z)
{
	return ((X & Y) | (~ X & Z));
}

void intToBigEndian(unsigned n, unsigned char *out)
{
	*out ++ = n >> 24;
	*out ++ = n >> 16;
	*out ++ = n >> 8;
	*out ++ = n;
}

void sm3_updateBlock(struct sm3_context *ctx)
{
	#define T_00_15	0x79cc4519
	#define T_16_63	0x7a879d8a
	unsigned i, SS1, SS2, TT1, TT2, aaa;
	unsigned vv_[8], ww_[64];

	memcpy( vv_, ctx->vv, sizeof(vv_) );
	for (i = 16; i < 68; i++)
		ctx->ww[i] = P1(ctx->ww[i - 16] ^ ctx->ww[i - 9] ^ (ROTATE(ctx->ww[i - 3], 15))) ^ (ROTATE(ctx->ww[i - 13], 7)) ^ ctx->ww[i - 6];

	for (i = 0; i < 64; i++)
		ww_[i] = ctx->ww[i] ^ ctx->ww[i + 4];

	for (i = 0; i < 16; i++)
	{
		aaa = ROTATE(vv_[0], 12);
		SS1 = aaa + vv_[4] + ROTATE(T_00_15, i);
		SS1 = ROTATE(SS1, 7);
		SS2 = SS1 ^ aaa;

		TT1 = FF_00_15(vv_[0], vv_[1], vv_[2]) + vv_[3] + SS2 + ww_[i];
		TT2 = GG_00_15(vv_[4], vv_[5], vv_[6]) + vv_[7] + SS1 + ctx->ww[i];
		vv_[3] = vv_[2];
		vv_[2] = ROTATE(vv_[1], 9);
		vv_[1] = vv_[0];
		vv_[0] = TT1;
		vv_[7] = vv_[6];
		vv_[6] = ROTATE(vv_[5], 19);
		vv_[5] = vv_[4];
		vv_[4] = P0(TT2);
	}

	for (i = 16; i < 64; i++)
	{
		aaa = ROTATE(vv_[0], 12);
		SS1 = aaa + vv_[4] + ROTATE(T_16_63, i);
		SS1 = ROTATE(SS1, 7);
		SS2 = SS1 ^ aaa;

		TT1 = FF_16_63(vv_[0], vv_[1], vv_[2]) + vv_[3] + SS2 + ww_[i];
		TT2 = GG_16_63(vv_[4], vv_[5], vv_[6]) + vv_[7] + SS1 + ctx->ww[i];
		vv_[3] = vv_[2];
		vv_[2] = ROTATE(vv_[1], 9);
		vv_[1] = vv_[0];
		vv_[0] = TT1;
		vv_[7] = vv_[6];
		vv_[6] = ROTATE(vv_[5], 19);
		vv_[5] = vv_[4];
		vv_[4] = P0(TT2);
	}

	for (i = 0; i < 8; i++)
		ctx->vv[i] ^= vv_[i];

	ctx->wordOff = 0;
	memset(ctx->ww, 0, 16 * 4);
}

void sm3_updateWord(struct sm3_context *ctx, const unsigned char *in)
{
	unsigned n = *in ++ << 24;
	n |= (*in ++ & 0xff) << 16;
	n |= (*in ++ & 0xff) << 8;
	n |= (*in ++ & 0xff);
	ctx->ww[ ctx->wordOff ] = n;

	if (++ctx->wordOff == 16)
		sm3_updateBlock(ctx);
}

void sm3_updateByte(struct sm3_context *ctx, unsigned char input)
{
	ctx->byteBuf[ctx->byteOff ++] = input;

	if (ctx->byteOff >= 4)
	{
		sm3_updateWord(ctx, ctx->byteBuf);
		ctx->byteOff = 0;
	}
	ctx->byteCount++;
}


int sm3_init(struct sm3_context *ctx)
{
	memset(ctx, 0, sizeof(struct sm3_context));
	memcpy(ctx->vv, sm3_iv, sizeof(sm3_iv));
	return 1;
}

int sm3_update(struct sm3_context *ctx, const unsigned char *data, size_t length)
{
	while ((ctx->byteOff != 0) && (length > 0))
	{
		sm3_updateByte(ctx, *data ++);
		length--;
	}
	while (length > 4)
	{
		sm3_updateWord(ctx, data);
		data += 4;
		length -= 4;
		ctx->byteCount += 4;
	}
	while (length > 0)
	{
		sm3_updateByte(ctx, *data ++);
		length--;
	}
	return 1;
}

int sm3_final(struct sm3_context *ctx, unsigned char *md)
{
	int i = ctx->byteCount << 3;
	sm3_updateByte(ctx, 128);
	while (ctx->byteOff)
		sm3_updateByte(ctx, 0);
	if (ctx->wordOff > 14)
		sm3_updateBlock(ctx);
	ctx->ww[ 15 ] = i;
	sm3_updateBlock(ctx);
	for (i = 0; i < 8; i++)
		intToBigEndian(ctx->vv[i], md + 4 * i);
	return 1;
}
