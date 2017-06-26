#ifndef __SM3_H__
#define	__SM3_H__

struct sm3_context
{
	unsigned byteOff, wordOff;
	unsigned byteCount;
	unsigned vv[8], ww[68];
	unsigned char byteBuf[ 4 ];
};

#ifdef __cplusplus
extern "C" {
#endif

unsigned ROTATE(unsigned x, unsigned n);
unsigned P0(unsigned X);
unsigned P1(unsigned X);
unsigned FF_00_15(unsigned X, unsigned Y, unsigned Z);
unsigned FF_16_63(unsigned X, unsigned Y, unsigned Z);
unsigned GG_00_15(unsigned X, unsigned Y, unsigned Z);
unsigned GG_16_63(unsigned X, unsigned Y, unsigned Z);
void intToBigEndian(unsigned n, unsigned char *out);
void sm3_updateBlock(struct sm3_context *ctx);
void sm3_updateWord(struct sm3_context *ctx, const unsigned char *in);
void sm3_updateByte(struct sm3_context *ctx, unsigned char input);

int sm3_init(struct sm3_context *ctx);
int sm3_update(struct sm3_context *ctx, const unsigned char *data, size_t length);
int sm3_final(struct sm3_context *ctx, unsigned char *md);
#ifdef __cplusplus
}
#endif
#endif