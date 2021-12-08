#ifndef __GFC_TYPES_H__
#define __GFC_TYPES_H__

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define GFC_EPSILON   1e-6f
#define GFC_ROOT2  1.414213562
#define GFC_2PI 6.283185308
#define GFC_PI 3.141592654
#define GFC_HALF_PI 1.570796327
/* conversion factor for converting from radians to degrees*/
#define GFC_RADTODEG  57.295779513082

/* conversion factor for converting from degrees to radians*/
#define GFC_DEGTORAD  0.017453292519943295769236907684886

typedef short unsigned int Bool;

#ifndef false
#define false 0
#endif
#ifndef true
#define true 1
#endif

/**
 * @brief random macro taken from Id Software's Quake 2 Source.
 * This macro exposes the more random bits from the rand() function
 * @return a random float between 0 and 1.0
 */
#define gfc_random()  ((rand ()%1000)/(float)1000.0)

/**
 * @brief random macro taken from Id Software's Quake 2 Source.
 * This macro exposes the more random bits from the rand() function
 * @return a random float between -1.0 and 1.0
 */
#define gfc_crandom() (((float)((rand()%1000)/(float)1000.0) * 2.0) - 1.0)

/**
 * basic operations
 */
#ifndef MIN
#define MIN(a,b)          (a<=b?a:b)
#endif

#ifndef MAX
#define MAX(a,b)          (a>=b?a:b)
#endif

#define gfc_rect_set(r,a,b,c,d) (r.x=(a), r.y=(b), r.w=(c), r.h=(d))

/**
 * @brief helper function to allocate and initialize arrays.  The array returned needs to be free'd
 * @param typeSize the size of the type for the array
 * @param count the number of elements in the array
 * @returns NULL on error (check logs), an array allocated and initialized to zero otherwise
 */
void *gfc_allocate_array(size_t typeSize,size_t count);

//#ifndef snprintf
//#define snprintf _snprintf
//#endif

#endif
