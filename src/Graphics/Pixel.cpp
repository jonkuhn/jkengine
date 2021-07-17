#include "Pixel.h"

std::ostream& operator<<(std::ostream &strm, const Pixel &p)
{
    return strm << "Pixel(" << +p.r << ", " << +p.g << ", " << +p.b << ", " << +p.a << ")";
}
