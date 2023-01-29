#include "Scene.h"

#include <memory>

#include "Aabb.h"
#include "SnapshotOfReadOnlyAabb2d.h"

using namespace JkEng::Physics;

Scene::Scene(const SceneDefinition& definition)
{
    auto& movableAabbDefinitions = definition.MovableAabb2dDefinitions();
    _aabbs.reserve(movableAabbDefinitions.size());
    for (auto& movableAabb2dDefinition : movableAabbDefinitions)
    {
        _aabbs.emplace_back(
            movableAabb2dDefinition.Position(),
            movableAabb2dDefinition.Size(),
            glm::vec2(),
            glm::vec2(),
            movableAabb2dDefinition.CollisionHandler(),
            movableAabb2dDefinition.ObjectInfo());

        // This pointer to the vector memory will be used externally
        // but it is safe because the vector will never be resized
        // because capacity was reserved above and we will only
        // add that many items to it in this loop.
        // TODO: Consider creating a wrapper for std::vector that
        // enforces this.
        movableAabb2dDefinition.SetAfterCreatePtr(&(_aabbs.back()));
    }
}

void Scene::Update(float deltaTime)
{
    for (float t = 0.0f; t < deltaTime; t += IScene::StepTime)
    {
        for (auto& aabb : _aabbs)
        {
            aabb.Velocity(aabb.Velocity() + aabb.Acceleration() * IScene::StepTime);
            aabb.Position(aabb.Position() + aabb.Velocity() * IScene::StepTime);
        }
    }

    size_t aabbCount = _aabbs.size();
    for (size_t outerIndex = 0; outerIndex < aabbCount; outerIndex++)
    {
        auto& aabb0 = _aabbs[outerIndex];
        for (size_t innerIndex = outerIndex + 1; innerIndex < aabbCount; innerIndex++)
        {
            auto& aabb1 = _aabbs[innerIndex];
            if (aabb0.IsColliding(aabb1))
            {
                SnapshotOfReadOnlyAabb2d snapshotOfAabb0(aabb0);
                SnapshotOfReadOnlyAabb2d snapshotOfAabb1(aabb1);
                aabb0.CollisionHandler()(snapshotOfAabb1);
                aabb1.CollisionHandler()(snapshotOfAabb0);
            }
        }
    }
}
