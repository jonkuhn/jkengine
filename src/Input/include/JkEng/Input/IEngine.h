#pragma once
#include <vector>

#include "IGamepad.h"

namespace JkEng::Input
{
    class IEngine
    {
    public:
        virtual void Update() = 0;
        virtual std::vector<IGamepad*> EnumerateGamepads() = 0;
    };
}
