#pragma once

struct color3
{
    color3() = default;
    color3(float rr, float gg, float bb, float aa=1.0f):
        r(rr), g(gg), b(bb), a(aa) {}
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 1.0f;
};
