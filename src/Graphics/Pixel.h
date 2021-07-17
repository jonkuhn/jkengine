#pragma once
#include <iostream>

struct Pixel
{
    uint8_t r, g, b, a;

    Pixel()
        : r(0), g(0), b(0), a(0)
    {
    }

    Pixel(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_)
        : r(r_), g(g_), b(b_), a(a_)
    {
    }

    bool operator==(const Pixel &other) const
    {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
};

std::ostream& operator<<(std::ostream &strm, const Pixel &p);
