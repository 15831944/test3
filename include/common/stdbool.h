#ifndef __STDBOOL_H__
#define __STDBOOL_H__

/*
stdbool.h 
 
-- Boolean type and values
(substitute for missing C99 standard header)
 
public-domain implementation from [EMAIL PROTECTED]
 
implements subclause 7.16 of ISO/IEC 9899:1999 (E)
*/
 
/* program is allowed to contain its own definitions, so ... */
#undef bool
#undef true
#undef false

#if 0
#if __STDC_VERSION__ < 199901
typedef int _Bool
#endif
#endif
 
#define bool int
#define true 1
#define false 0

#endif