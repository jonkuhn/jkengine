#include "Color.h"

std::ostream& JkEng::Graphics::operator<<(std::ostream &strm, const JkEng::Graphics::Color &p)
{
    return strm << "Color(" << +p.r << ", " << +p.g << ", " << +p.b << ", " << +p.a << ")";
}
