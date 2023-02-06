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
        void Update(float deltaTime) override;
        float TimeNotYetSimulated() override { return _timeNotYetSimulated; }

    private:
        float _timeNotYetSimulated;

        // Performance Note: It is substantially faster to keep all Aabbs
        // contiguous in memory versus doing std::vector<unique_ptr<Aabb>>
        // The latter makes collision checks for 1000 objects about ~50-55%
        // longer.
        std::vector<Aabb> _aabbs;
    };
}
