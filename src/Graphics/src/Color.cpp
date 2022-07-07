#include "Color.h"

std::ostream& Graphics::operator<<(std::ostream &strm, const Graphics::Color &p)
{
    return strm << "Color(" << +p.r << ", " << +p.g << ", " << +p.b << ", " << +p.a << ")";
}
