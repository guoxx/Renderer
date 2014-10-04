#ifndef __RENDERER__FRAGMENT__
#define __RENDERER__FRAGMENT__

#include "utMath.h"

typedef struct Fragment {
    Vec3f normal;
    Vec3f color;
    Vec3f vEye;

    // position on screen
    float x, y;
    float depth;
} Fragment;

#endif /* defined(__RENDERER__FRAGMENT__) */
