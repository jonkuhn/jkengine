#pragma once
#include <iostream>

namespace Graphics
{
    struct Color
    {
        uint8_t r, g, b, a;

        Color()
            : r(0), g(0), b(0), a(0)
        {
        }

        Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_)
            : r(r_), g(g_), b(b_), a(a_)
        {
        }

        bool operator==(const Color &other) const
        {
            return r == other.r && g == other.g && b == other.b && a == other.a;
        }
    };

    std::ostream& operator<<(std::ostream &strm, const Graphics::Color &p);
}