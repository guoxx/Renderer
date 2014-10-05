#ifndef __RENDERER__FRAGMENT__
#define __RENDERER__FRAGMENT__

#include "utMath.h"

typedef struct Fragment {
    /** The user-interpolated normal */
    Vec3f normal;

    /** The user-interpolated color */
    Vec3f color;

    /** The user-interpolated texture coordinate */
    Vec2f texCoord;

    /** The user-interpolated eye-space position.  Use this to do lighting calculations in the fragment shader */
    Vec3f vEye;

    /** The screen space coordinate of this fragment. */
    int x, y;

    /** the depth coordinate */
    float depth;
} Fragment;

#endif /* defined(__RENDERER__FRAGMENT__) */
