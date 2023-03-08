#pragma once

namespace JkEng::Graphics
{
    class GridLocation
    {
    public:
        unsigned int x;
        unsigned int y;

        GridLocation()
            : x(0), y(0) { }
        GridLocation(unsigned int x_, unsigned int y_)
            : x(x_), y(y_) { }

        bool operator==(const GridLocation &other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const GridLocation &other) const
        {
            return !operator==(other);
        }
    };
}
