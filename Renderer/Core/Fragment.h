#pragma once

#include <type_traits>
#include "utMath.h"

class Fragment
{
public:
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
};