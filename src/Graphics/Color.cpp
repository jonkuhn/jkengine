#include "Color.h"

std::ostream& operator<<(std::ostream &strm, const Color &p)
{
    return strm << "Color(" << +p.r << ", " << +p.g << ", " << +p.b << ", " << +p.a << ")";
}
