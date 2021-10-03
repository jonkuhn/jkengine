#pragma once

#include <vector>

#include "MovableAabb2dDefinition.h"

namespace JkEng::Physics
{
    class SceneDefinition final
    {
    public:
        inline void AddMovableAabb2d(MovableAabb2dDefinition movableAabb2dDefinitions)
        {
            _movableAabb2dDefinitions.push_back(std::move(movableAabb2dDefinitions));
        }

        inline const std::vector<MovableAabb2dDefinition>& MovableAabb2dDefinitions() const
        {
            return _movableAabb2dDefinitions;
        }

    private:
        std::vector<MovableAabb2dDefinition> _movableAabb2dDefinitions;
    };
}
