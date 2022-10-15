#pragma once

namespace JkEng::Graphics
{
    class GridLocation
    {
    public:
        GridLocation()
            : x(0), y(0) { }
        GridLocation(unsigned int x_, unsigned int y_)
            : x(x_), y(y_) { }
        unsigned int x;
        unsigned int y;
    };
}
