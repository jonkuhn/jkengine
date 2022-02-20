#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>

#include "Shared/Registry.h"

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


    ASSERT_EQ(iteratedObjects.size(), static_cast<size_t>(2));
    ASSERT_TRUE(iteratedObjects.contains(obj0.get()));
    ASSERT_FALSE(iteratedObjects.contains(obj1.get()));
    ASSERT_TRUE(iteratedObjects.contains(obj2.get()));
}

TEST_F(RegistryTests, ForEach_GivenRegisteredObjectDestructedDuringCallback_AbortsProcess)
{
    Registry<DummyObject> registry;

    auto obj0 = std::make_unique<DummyObject>(registry);
    auto obj1 = std::make_unique<DummyObject>(registry);
    auto obj2 = std::make_unique<DummyObject>(registry);

    std::unordered_set<DummyObject*> iteratedObjects;

    // We cannot catch the std::logic_error because it will be thrown
    // in a destructor
    ASSERT_DEATH(
        registry.ForEach(
            [&](DummyObject& obj)
            {
                if(&obj == obj1.get())
                {
                    obj1.reset();
                }
            }
        ),
        "Calling Registry::Deregister\\(\\) from ForEach callback is currently not supported"
    );
}

TEST_F(RegistryTests, ForEach_GivenNewObjectRegisteredDuringCallback_ThrowsLogicError)
{
    Registry<DummyObject> registry;

    auto obj0 = std::make_unique<DummyObject>(registry);
    auto obj1 = std::make_unique<DummyObject>(registry);
    auto obj2 = std::make_unique<DummyObject>(registry);

    std::unordered_set<DummyObject*> iteratedObjects;

    std::unique_ptr<DummyObject> obj3;
    ASSERT_THROW(
        registry.ForEach(
            [&](DummyObject&)
            {
                obj3 = std::make_unique<DummyObject>(registry);
            }
        ),
        std::logic_error
    );
}

TEST_F(RegistryTests, ForEach_GivenCallbackCallsForEach_ThrowsLogicError)
{
    Registry<DummyObject> registry;

    auto obj0 = std::make_unique<DummyObject>(registry);
    auto obj1 = std::make_unique<DummyObject>(registry);
    auto obj2 = std::make_unique<DummyObject>(registry);

    std::vector<std::pair<DummyObject*, DummyObject*>> iteratedObjects;

    std::unique_ptr<DummyObject> obj3;
    ASSERT_THROW(
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
        ),
        std::logic_error
    );
}