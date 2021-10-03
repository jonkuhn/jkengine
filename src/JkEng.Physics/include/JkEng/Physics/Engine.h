#pragma once

#include <memory>

#include "IMovableAabb2d.h"
#include "IScene.h"
#include "SceneDefinition.h"

namespace JkEng::Physics
{
    class Scene;

    class Engine
    {
    public:
        std::unique_ptr<IScene> CreateScene(const SceneDefinition& definition);
    };
}
