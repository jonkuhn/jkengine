#pragma once
#include <vector>

#include "IGamepad.h"

namespace Input
{
    class IEngine
    {
    public:
        virtual void Update() = 0;
        virtual std::vector<IGamepad*> EnumerateGamepads() = 0;
    };
}
