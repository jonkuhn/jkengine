#include <chrono>
#include <cstdint>
#include <memory>
#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <JkEng/Physics/Engine.h>
#include "Aabb.h"

using namespace testing;
using namespace JkEng;
using namespace JkEng::Physics;

class MovableAabb2dTests : public Test
{
public:
    MovableAabb2dTests()
    {

    }
protected:
    Engine _engine;
};

TEST_F(MovableAabb2dTests, Update_1000NonOverlappingMovableAabb2ds)
{
    const int objectCount = 1000;
    const int updateCount = 6000;

    auto start = std::chrono::high_resolution_clock::now();
    SceneDefinition sceneDefinition;

    for (int i = 0; i < objectCount; i++)
    {
        sceneDefinition.AddMovableAabb2d(
            MovableAabb2dDefinition(
                nullptr,
                glm::vec2(i * 6.0f, 0.0f),
                glm::vec2(5.0f, 10.0f),
                [&](const IReadOnlyAabb2d&) { },
                std::any()
            )
        );
    };

    auto scene = _engine.CreateScene(sceneDefinition);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Scene create time: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us"
        << " sizeof(float) = " << sizeof(float) << " << sizeof(Aabb) = " << sizeof(Aabb)
        << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < updateCount; i++)
    {
        scene->Update(0.0f);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Update " << updateCount << " times: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us"
        << std::endl;
}

TEST_F(MovableAabb2dTests, Update_1000OverlappingMovableAabb2dsWith1998Collisions)
{
    const int objectCount = 1000;
    const int updateCount = 6000;

    auto start = std::chrono::high_resolution_clock::now();
    SceneDefinition sceneDefinition;

    for (int i = 0; i < objectCount; i++)
    {
        sceneDefinition.AddMovableAabb2d(
            MovableAabb2dDefinition(
                nullptr,
                glm::vec2(i * 3.0f, 0.0f),
                glm::vec2(5.0f, 10.0f),
                [&](const IReadOnlyAabb2d&) { },
                std::any()
            )
        );
    };

    auto scene = _engine.CreateScene(sceneDefinition);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Scene create time: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us"
        << " sizeof(float) = " << sizeof(float) << " << sizeof(Aabb) = " << sizeof(Aabb)
        << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < updateCount; i++)
    {
        scene->Update(0.0f);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Update " << updateCount << " times: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us"
        << std::endl;
}