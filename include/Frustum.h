#pragma once

#include "Plane.h"

struct Frustum
{
    enum { Left, Right, Bottom, Top, Near, Far, Frustrum_Plane_Count };
    PlaneF plane[Frustrum_Plane_Count];
};
