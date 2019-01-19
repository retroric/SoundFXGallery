
/*
 * types.h - common custom type definitions
 */

#ifndef __TYPES_H
#define __TYPES_H


/* ================== TYPES ================== */
typedef unsigned char uchar;
typedef char           bool;

/* ================== CONSTANTES ================== */
#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef FALSE
#define FALSE ((bool) 0)
#define TRUE  (! FALSE)
#endif

#endif // __TYPES_H
