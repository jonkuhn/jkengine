#include <gtest/gtest.h>

#include <JkEng/Physics/Engine.h>

using namespace testing;
using namespace JkEng;
using namespace JkEng::Physics;

class EngineTests : public Test
{
public:
    EngineTests()
    {

    }
protected:
    Engine _engine;
};

TEST_F(EngineTests, CreateScene_GivenNullAfterCreatePtr_DoesNotAbortOrThrow)
{
    SceneDefinition sceneDefinition;
    sceneDefinition.AddMovableAabb2d(
        MovableAabb2dDefinition(
            nullptr,
            glm::vec2(50.0f, 25.0f),
            glm::vec2(5.0f, 10.0f),
            [&](const IReadOnlyAabb2d&) { },
            std::any()
        )
    );

    ASSERT_NO_THROW(_engine.CreateScene(sceneDefinition));
}

TEST_F(EngineTests, Update_GivenSingleMovableAabb2d_CollisionHandlerIsNotCalledForCollisionWithSelf)
{
    bool collisionHandler0WasCalled = false;

    SceneDefinition sceneDefinition;
    AfterCreatePtr<IMovableAabb2d> movableAabb0;
    sceneDefinition.AddMovableAabb2d(
        MovableAabb2dDefinition(
            &movableAabb0,
            glm::vec2(50.0f, 25.0f),
            glm::vec2(5.0f, 10.0f),
            [&](const IReadOnlyAabb2d&)
            {
                collisionHandler0WasCalled = true;
            },
            std::any()
        )
    );

    auto scene = _engine.CreateScene(sceneDefinition);
    scene->Update();

    ASSERT_FALSE(collisionHandler0WasCalled);
}

TEST_F(EngineTests, Update_GivenTwoNonOverlappingMovableAabb2d_NoCollisionHandlerIsCalled)
{
    glm::vec2 position0(50.0f, 25.0f);
    glm::vec2 size0(5.0f, 10.0f);
    bool collisionHandler0WasCalled = false;

    SceneDefinition sceneDefinition;
    AfterCreatePtr<IMovableAabb2d> movableAabb0;
    sceneDefinition.AddMovableAabb2d(
        MovableAabb2dDefinition(
            &movableAabb0,
            position0,
            size0,
            [&](const IReadOnlyAabb2d&)
            {
                collisionHandler0WasCalled = true;
            },
            std::any()
        )
    );

    // moveableAabb0 does not collide with moveableAabb1 because
    // we set the position of "1" to be the position of "0" plus
    // more than the size of "0" in both the x and y dimensions.
    glm::vec2 position1(
        position0.x + size0.x + 1.0f,
         position0.y + size0.y + 1.0f);
    glm::vec2 size1 = size0;
    bool collisionHandler1WasCalled = false;

    AfterCreatePtr<IMovableAabb2d> movableAabb1;
    sceneDefinition.AddMovableAabb2d(
        MovableAabb2dDefinition(
            &movableAabb1,
            position1,
            size1,
            [&](const IReadOnlyAabb2d&)
            {
                collisionHandler1WasCalled = true;
            },
            std::any()
        )
    );

    auto scene = _engine.CreateScene(sceneDefinition);
    scene->Update();

    ASSERT_FALSE(collisionHandler0WasCalled);
    ASSERT_FALSE(collisionHandler1WasCalled);
}

TEST_F(EngineTests, Update_GivenTwoOverlappingMovableAabb2d_BothCollisionHandlersAreCalledExactlyOnce)
{
    glm::vec2 position0(50.0f, 25.0f);
    glm::vec2 size0(5.0f, 10.0f);
    int collisionHandler0CallCount = 0;

    SceneDefinition sceneDefinition;
    AfterCreatePtr<IMovableAabb2d> movableAabb0;
    sceneDefinition.AddMovableAabb2d(
        MovableAabb2dDefinition(
            &movableAabb0,
            position0,
            size0,
            [&](const IReadOnlyAabb2d&)
            {
                collisionHandler0CallCount++;
            },
            std::any()
        )
    );

    // moveableAabb0 collides with moveableAabb1 because
    // we set the position of "1" to be the position of "0" plus
    // only half the size of "0" in both the x and y dimensions.
    glm::vec2 position1(
        position0.x + size0.x / 2.0f,
         position0.y + size0.y / 2.0f);
    glm::vec2 size1 = size0;
    int collisionHandler1CallCount = 0;

    AfterCreatePtr<IMovableAabb2d> movableAabb1;
    sceneDefinition.AddMovableAabb2d(
        MovableAabb2dDefinition(
            &movableAabb1,
            position1,
            size1,
            [&](const IReadOnlyAabb2d&)
            {
                collisionHandler1CallCount++;
            },
            std::any()
        )
    );

    auto scene = _engine.CreateScene(sceneDefinition);
    scene->Update();

    ASSERT_EQ(collisionHandler0CallCount, 1);
    ASSERT_EQ(collisionHandler1CallCount, 1);
}

TEST_F(EngineTests, Update_GivenTwoOverlappingMovableAabb2d_CollisionHandlersPassedArgumentWithOppositeAabbSizeAndPositionBeforeCallbackSideEffects)
{
    SceneDefinition sceneDefinition;

    glm::vec2 position0(50.0f, 25.0f);
    glm::vec2 size0(5.0f, 10.0f);

    glm::vec2 aabb0CallbackArgPosition;
    glm::vec2 aabb0CallbackArgSize;

    AfterCreatePtr<IMovableAabb2d> movableAabb0;
    sceneDefinition.AddMovableAabb2d(
        MovableAabb2dDefinition(
            &movableAabb0,
            position0,
            size0,
            [&](const IReadOnlyAabb2d& otherAabb)
            {
                aabb0CallbackArgPosition = otherAabb.Position();
                aabb0CallbackArgSize = otherAabb.Size();

                // callback side effects set the position and
                // size of aabb0 (the object this callback is for)
                movableAabb0->Position(glm::vec2(0.0f, 0.0f));
                movableAabb0->Size(glm::vec2(0.0f, 0.0f));
            },
            std::any()
        )
    );

    // moveableAabb0 collides with moveableAabb1 because
    // we set the position of "1" to be the position of "0" plus
    // only half the size of "0" in both the x and y dimensions.
    glm::vec2 position1(
        position0.x + size0.x / 2.0f,
        position0.y + size0.y / 2.0f);
    glm::vec2 size1(
        size0.x + 10.0f,
        size0.y + 10.0f);

    glm::vec2 aabb1CallbackArgPosition;
    glm::vec2 aabb1CallbackArgSize;

    AfterCreatePtr<IMovableAabb2d> movableAabb1;
    sceneDefinition.AddMovableAabb2d(
        MovableAabb2dDefinition(
            &movableAabb1,
            position1,
            size1,
            [&](const IReadOnlyAabb2d& otherAabb)
            {
                aabb1CallbackArgPosition = otherAabb.Position();
                aabb1CallbackArgSize = otherAabb.Size();

                // callback side effects set the position and
                // size of aabb1 (the object this callback is for)
                movableAabb1->Position(glm::vec2(1.0f, 1.0f));
                movableAabb1->Size(glm::vec2(1.0f, 1.0f));
            },
            std::any()
        )
    );

    auto scene = _engine.CreateScene(sceneDefinition);
    scene->Update();

    // aabb0's callback will get the position and size of aabb1
    ASSERT_EQ(aabb0CallbackArgPosition, position1);
    ASSERT_EQ(aabb0CallbackArgSize, size1);

    // aabb1's callback will get the position and size of aabb0
    ASSERT_EQ(aabb1CallbackArgPosition, position0);
    ASSERT_EQ(aabb1CallbackArgSize, size0);
}

TEST_F(EngineTests, Update_GivenTwoOverlappingMovableAabb2dAndNullptrCollisionHandlers_DoesNotThrowOrAbort)
{
    SceneDefinition sceneDefinition;

    glm::vec2 position0(50.0f, 25.0f);
    glm::vec2 size0(5.0f, 10.0f);

    AfterCreatePtr<IMovableAabb2d> movableAabb0;
    sceneDefinition.AddMovableAabb2d(
        MovableAabb2dDefinition(
            &movableAabb0,
            position0,
            size0,
            nullptr,
            std::any()
        )
    );

    // moveableAabb0 collides with moveableAabb1 because
    // we set the position of "1" to be the position of "0" plus
    // only half the size of "0" in both the x and y dimensions.
    glm::vec2 position1(
        position0.x + size0.x / 2.0f,
        position0.y + size0.y / 2.0f);
    glm::vec2 size1(
        size0.x + 10.0f,
        size0.y + 10.0f);

    AfterCreatePtr<IMovableAabb2d> movableAabb1;
    sceneDefinition.AddMovableAabb2d(
        MovableAabb2dDefinition(
            &movableAabb1,
            position1,
            size1,
            nullptr,
            std::any()
        )
    );

    auto scene = _engine.CreateScene(sceneDefinition);
    ASSERT_NO_THROW(scene->Update());
}
