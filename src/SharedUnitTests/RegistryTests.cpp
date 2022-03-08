#include <algorithm>
#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>

#include "jkengine/Shared/Registry.h"

using namespace testing;
using namespace Shared;

class DummyObject
{
public:
    DummyObject(Registry<DummyObject>& registry)
      : _registration(registry, *this)
    {

    }

private:
    Registry<DummyObject>::Registration _registration;
};

class RegistryTests : public Test
{
public:
    RegistryTests()
    {

    }
};

template<typename T>
void AssertVectorContains(std::vector<T> vec, T objectToFind)
{
    ASSERT_TRUE(
        std::any_of(
            vec.cbegin(),
            vec.cend(),
            [&](T o)
            {
                return o == objectToFind;
            }
        )
    );
}

TEST_F(RegistryTests, ForEach_GivenObjectsRegistered_CallsCallbackForEachObject)
{
    Registry<DummyObject> registry;

    DummyObject obj0(registry);
    DummyObject obj1(registry);
    DummyObject obj2(registry);

    std::unordered_set<DummyObject*> iteratedObjects;

    registry.ForEach(
        [&](DummyObject& obj)
        {
            iteratedObjects.insert(&obj);
        }
    );

    ASSERT_EQ(iteratedObjects.size(), static_cast<size_t>(3));
    ASSERT_TRUE(iteratedObjects.contains(&obj0));
    ASSERT_TRUE(iteratedObjects.contains(&obj1));
    ASSERT_TRUE(iteratedObjects.contains(&obj2));
}

TEST_F(RegistryTests, ForEach_GivenRegisteredObjectDestructed_DoesNotCallCallbackForIt)
{
    Registry<DummyObject> registry;

    auto obj0 = std::make_unique<DummyObject>(registry);
    auto obj1 = std::make_unique<DummyObject>(registry);
    auto obj2 = std::make_unique<DummyObject>(registry);

    std::unordered_set<DummyObject*> iteratedObjects;

    obj1.reset();

    registry.ForEach(
        [&](DummyObject& obj)
        {
            iteratedObjects.insert(&obj);
        }
    );

    // Must contain objects that were not destructed
    ASSERT_EQ(iteratedObjects.size(), static_cast<size_t>(2));
    ASSERT_TRUE(iteratedObjects.contains(obj0.get()));
    ASSERT_TRUE(iteratedObjects.contains(obj2.get()));

    // Must not contain object that was destructed
    ASSERT_FALSE(iteratedObjects.contains(obj1.get()));
}

TEST_F(RegistryTests, ForEach_GivenRegisteredObjectDestructedDuringCallback_NextForEachDoesNotIncludeIt)
{
    Registry<DummyObject> registry;

    auto obj0 = std::make_unique<DummyObject>(registry);
    auto obj1 = std::make_unique<DummyObject>(registry);
    auto obj2 = std::make_unique<DummyObject>(registry);


    registry.ForEach(
        [&](DummyObject& obj)
        {
            if(&obj == obj1.get())
            {
                obj1.reset();
            }
        }
    );

    std::unordered_set<DummyObject*> iteratedObjects;
    registry.ForEach(
        [&](DummyObject& obj)
        {
            iteratedObjects.insert(&obj);
        }
    );

    ASSERT_EQ(iteratedObjects.size(), static_cast<size_t>(2));
    ASSERT_TRUE(iteratedObjects.contains(obj0.get()));
    ASSERT_TRUE(iteratedObjects.contains(obj2.get()));
}

TEST_F(RegistryTests, ForEach_GivenNewObjectRegisteredDuringCallback_NextForEachDoesIncludeIt)
{
    Registry<DummyObject> registry;

    auto obj0 = std::make_unique<DummyObject>(registry);
    auto obj1 = std::make_unique<DummyObject>(registry);
    auto obj2 = std::make_unique<DummyObject>(registry);

    std::unique_ptr<DummyObject> obj3;
    registry.ForEach(
        [&](DummyObject&)
        {
            obj3 = std::make_unique<DummyObject>(registry);
        }
    );

    std::unordered_set<DummyObject*> iteratedObjects;
    registry.ForEach(
        [&](DummyObject& obj)
        {
            iteratedObjects.insert(&obj);
        }
    );

    ASSERT_EQ(iteratedObjects.size(), static_cast<size_t>(4));
    ASSERT_TRUE(iteratedObjects.contains(obj0.get()));
    ASSERT_TRUE(iteratedObjects.contains(obj1.get()));
    ASSERT_TRUE(iteratedObjects.contains(obj2.get()));
    ASSERT_TRUE(iteratedObjects.contains(obj3.get()));
}

TEST_F(RegistryTests, ForEach_GivenCallbackCallsForEach_InnerAndOuterLoopOverSameObjects)
{
    Registry<DummyObject> registry;

    auto obj0 = std::make_unique<DummyObject>(registry);
    auto obj1 = std::make_unique<DummyObject>(registry);
    auto obj2 = std::make_unique<DummyObject>(registry);

    std::vector<std::pair<DummyObject*, DummyObject*>> iteratedObjects;

    std::unique_ptr<DummyObject> obj3;
    registry.ForEach(
        [&](DummyObject& outerObj)
        {
            registry.ForEach(
                [&](DummyObject& innerObj)
                {
                    iteratedObjects.push_back(std::make_pair(&outerObj, &innerObj));
                }
            );
        }
    );

    ASSERT_EQ(iteratedObjects.size(), static_cast<size_t>(9));
    AssertVectorContains(iteratedObjects, std::make_pair(obj0.get(), obj0.get()));
    AssertVectorContains(iteratedObjects, std::make_pair(obj0.get(), obj1.get()));
    AssertVectorContains(iteratedObjects, std::make_pair(obj0.get(), obj2.get()));
    AssertVectorContains(iteratedObjects, std::make_pair(obj1.get(), obj0.get()));
    AssertVectorContains(iteratedObjects, std::make_pair(obj1.get(), obj1.get()));
    AssertVectorContains(iteratedObjects, std::make_pair(obj1.get(), obj2.get()));
    AssertVectorContains(iteratedObjects, std::make_pair(obj2.get(), obj0.get()));
    AssertVectorContains(iteratedObjects, std::make_pair(obj2.get(), obj1.get()));
    AssertVectorContains(iteratedObjects, std::make_pair(obj2.get(), obj2.get()));
}