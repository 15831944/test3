#ifndef _TEST_IMAGES_H_
#define _TEST_IMAGES_H_

#define fourcc zbar_fourcc

#ifdef __cplusplus

extern "C" {
    int test_image_check_cleanup(void);
    int test_image_bars(zbar::zbar_image_t*);
    int test_image_ean13(zbar::zbar_image_t*);
}

#else

int test_image_check_cleanup(void);
int test_image_bars(zbar_image_t*);
int test_image_ean13(zbar_image_t*);

#endif

extern const char *test_image_ean13_data;

#endif