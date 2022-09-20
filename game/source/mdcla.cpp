// ======================================================================
// Title: mdcla.cpp
// Description: The implementation file for my personal linear algebra library.
// Last Modified: 9/20/2022 MDC
// ======================================================================

#include <mdcla.hpp>

// Struct Vec2F

Vec2F::Vec2F()
{
    x = 0;
    y = 0;
}

Vec2F::Vec2F(float _x, float _y)
{
    x = _x;
    y = _y;
}

float& Vec2F::operator [](int i)
{
    return *(&x + i);
}

const float& Vec2F::operator [](int i) const
{
    return *(&x + i);
}
