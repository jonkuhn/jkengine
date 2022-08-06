#include <algorithm>
#include <array>
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

class DestructionTrackingObject
{
public:
    DestructionTrackingObject(Registry<DestructionTrackingObject>& registry, bool& setWhenDestructorIsCalled)
      : _setWhenDestructorIsCalled(setWhenDestructorIsCalled),
        _registration(registry, *this)
    {

    }

    ~DestructionTrackingObject()
    {
        _setWhenDestructorIsCalled = true;
    }

private:
    bool& _setWhenDestructorIsCalled;
    Registry<DestructionTrackingObject>::Registration _registration;
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

TEST_F(RegistryTests, ForEach_GivenRegisteredObjectLaterInIterationIsDestructedDuringCallback_DestructedObjectsAreNotReturned)
{
    Registry<DestructionTrackingObject> registry;

    std::array<bool, 3> objectsDestructed = { false, false, false };
    std::array<std::unique_ptr<DestructionTrackingObject>, 3> objectsUniquePtr = {
        std::make_unique<DestructionTrackingObject>(registry, objectsDestructed[0]),
        std::make_unique<DestructionTrackingObject>(registry, objectsDestructed[1]),
        std::make_unique<DestructionTrackingObject>(registry, objectsDestructed[2])
    };
    
    // Save original pointer to each object so original memory address will be
    // accessible even if the unique_ptrs above are reset
    std::array<DestructionTrackingObject*, 3> objectsOriginalAddress = {
        objectsUniquePtr[0].get(),
        objectsUniquePtr[1].get(),
        objectsUniquePtr[2].get()
    };

    std::cout << "objectsOriginalAddress[0] = 0x" << std::ios::hex << objectsOriginalAddress[0] << std::endl;
    std::cout << "objectsOriginalAddress[1] = 0x" << std::ios::hex << objectsOriginalAddress[1] << std::endl;
    std::cout << "objectsOriginalAddress[2] = 0x" << std::ios::hex << objectsOriginalAddress[2] << std::endl;

    int iteration = 0;
    registry.ForEach(
        [&](DestructionTrackingObject& currentIterationObject)
        {
            std::cout << "iteration " << iteration
                << " &object = 0x " << std::ios::hex << &currentIterationObject << std::endl;

            // Since the order objects will be encountered by ForEach is not deterministic and not
            // guaranteed, on the first iteration delete all other registered objects to guarantee
            // a later iteration will hit a deleted object
            if (iteration == 0)
            {
                for (auto& objectUniquePtr : objectsUniquePtr)
                {
                    if (objectUniquePtr.get() != &currentIterationObject)
                    {
                        objectUniquePtr.reset();
                    }
                }
            }

            // The object passed to the ForEach callback must never be a destructed object
            for (std::size_t i = 0; i < objectsOriginalAddress.size(); i++)
            {
                if (&currentIterationObject == objectsOriginalAddress[i])
                {
                    ASSERT_FALSE(objectsDestructed[i]);
                }
            }

            iteration++;
        }
    );
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