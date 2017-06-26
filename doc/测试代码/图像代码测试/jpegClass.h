#ifndef __JPEGCLASS_H__
#define __JPEGCLASS_H__

#define JPEG_MACROS			0

#define PUT_2B(array,offset,value)  \
				(array[offset]  = (char) ((value) & 0xFF), \
				array[offset+1] = (char) (((value) >> 8) & 0xFF))
#define PUT_4B(array,offset,value)  \
				(array[offset]  = (char) ((value) & 0xFF), \
				array[offset+1] = (char) (((value) >> 8) & 0xFF), \
				array[offset+2] = (char) (((value) >> 16) & 0xFF), \
				array[offset+3] = (char) (((value) >> 24) & 0xFF))

#if (JPEG_MACROS == 1)

#include <libjpeg/jpeglib.h>
#pragma comment(lib, "libjpeg.lib")

void write_bmp_header(j_decompress_ptr cinfo, FILE *output_file);

void write_pixel_data(j_decompress_ptr cinfo, unsigned char *output_buffer, FILE *output_file);

int read_jpeg_file(const char *input_filename, const char *output_filename);
#endif

#endif