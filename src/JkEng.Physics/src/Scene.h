#pragma once

#include <vector>

#include "Aabb.h"
#include "IScene.h"
#include "SceneDefinition.h"

namespace JkEng::Physics
{
    class SceneDefinition;

    class Scene final : public IScene
    {
    public:
        Scene(const SceneDefinition& definition);
        void Update() override;

    private:
        std::vector<Aabb> _aabbs;
    };
}
