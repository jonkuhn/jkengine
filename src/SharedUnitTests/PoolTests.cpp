#include <algorithm>
#include <array>
#include <set>
#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "jkengine/Shared/Pool.h"

using namespace testing;
using namespace Shared;

class DummyObjectWithNonzeroSize
{
public:
    DummyObjectWithNonzeroSize()
      : _s("default value")
    {

    }

    inline const std::string& s() { return _s; }

private:
    std::string _s;
};

class DummyObjectWithConstructorArguments
{
public:
    DummyObjectWithConstructorArguments(int i, std::string s)
      : _i(i),
        _s(std::move(s))
    {

    }

    inline int i() { return _i; }
    inline const std::string& s() { return _s; }

private:
    int _i;
    std::string _s;
};

class DummyObjectWithLValueConstructorArgument
{
public:
    DummyObjectWithLValueConstructorArgument(int& i)
      : _i(i)
    {

    }

    inline int i() { return _i; }

private:
    int _i;
};

class DestructionTrackingObject
{
public:
    DestructionTrackingObject(bool& setWhenDestructorIsCalled)
      : _setWhenDestructorIsCalled(setWhenDestructorIsCalled)
    {

    }

    ~DestructionTrackingObject()
    {
        _setWhenDestructorIsCalled = true;
    }

private:
    bool& _setWhenDestructorIsCalled;
};

class PoolTests : public Test
{
public:
    PoolTests()
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


TEST_F(PoolTests, MakeUnique_GivenObjectWithOnlyPoolConstructorArgument_ReturnsObject)
{
    Pool<DummyObjectWithNonzeroSize> pool;

    auto obj = pool.MakeUnique();

    ASSERT_NE(obj.get(), nullptr);
}

TEST_F(PoolTests, MakeUnique_GivenObjectWithAdditionalNonPoolConstructorArguments_ReturnsObject)
{
    Pool<DummyObjectWithConstructorArguments> pool;

    auto obj = pool.MakeUnique(123, "abc");

    ASSERT_NE(obj.get(), nullptr);
}

TEST_F(PoolTests, MakeUnique_GivenObjectWithLValueConstructorArgument_ReturnsObject)
{
    Pool<DummyObjectWithLValueConstructorArgument> pool;

    int i = 123;
    auto obj = pool.MakeUnique(i);

    ASSERT_NE(obj.get(), nullptr);
}

TEST_F(PoolTests, MakeUnique_GivenTwoObjectsCreated_ObjectsAreAligned)
{
    Pool<DummyObjectWithNonzeroSize> pool;

    auto obj0 = pool.MakeUnique();
    auto obj1 = pool.MakeUnique();

    ASSERT_EQ(reinterpret_cast<uintptr_t>(obj0.get()) % alignof(DummyObjectWithNonzeroSize), 0u);
    ASSERT_EQ(reinterpret_cast<uintptr_t>(obj1.get()) % alignof(DummyObjectWithNonzeroSize), 0u);
}

TEST_F(PoolTests, MakeUnique_GivenTwoObjectsCreated_SecondObjectIsImmediatelyAfterFirstInMemory)
{
    Pool<DummyObjectWithNonzeroSize> pool;

    auto obj0 = pool.MakeUnique();
    auto obj1 = pool.MakeUnique();

    ASSERT_EQ(obj0.get() + 1, obj1.get());
}

TEST_F(PoolTests, MakeUnique_GivenInitialCapacityOf8ObjectsAndCalled9Times_AlwaysReturnsNonNull)
{
    static constexpr size_t CapacityPerChunk = 8;
    Pool<DummyObjectWithNonzeroSize, CapacityPerChunk> pool;
    std::vector<PoolUniquePtr<DummyObjectWithNonzeroSize>::T> keepObjectsInScope;

    ASSERT_NO_THROW(
        for (size_t i = 0; i < CapacityPerChunk + 1; i++)
        {
            auto obj = pool.MakeUnique();
            ASSERT_NE(obj.get(), nullptr);
            keepObjectsInScope.push_back(std::move(obj));
            std::cout << "Created object " << std::dec << i << std::endl;
        }
    );
}

TEST_F(PoolTests, MakeUnique_GivenInitialCapacityOf8ObjectsAndCalled9Times_AllAddressesAreUnique)
{
    static constexpr size_t CapacityPerChunk = 8;
    Pool<DummyObjectWithNonzeroSize, CapacityPerChunk> pool;
    std::vector<PoolUniquePtr<DummyObjectWithNonzeroSize>::T> keepObjectsInScope;
    std::set<DummyObjectWithNonzeroSize*> setOfUniquePointerValues;

    for (size_t i = 0; i < CapacityPerChunk + 1; i++)
    {
        auto obj = pool.MakeUnique();
        setOfUniquePointerValues.insert(obj.get());
        keepObjectsInScope.push_back(std::move(obj));
    }

    ASSERT_EQ(setOfUniquePointerValues.size(), CapacityPerChunk+1);
}

TEST_F(PoolTests, MakeUnique_GivenInitialCapacityOf8ObjectsAndCalled9Times_AddressesDidNotChange)
{
    static constexpr size_t CapacityPerChunk = 8;
    Pool<DummyObjectWithNonzeroSize, CapacityPerChunk> pool;
    std::vector<PoolUniquePtr<DummyObjectWithNonzeroSize>::T> objects;

    for (size_t i = 0; i < CapacityPerChunk + 1; i++)
    {
        auto obj = pool.MakeUnique();
        objects.push_back(std::move(obj));
    }

    // Assert that each initial address of an object is contained
    // in the list of objects iterated by ForEach
    std::unordered_set<DummyObjectWithNonzeroSize*> iteratedObjects;
    pool.ForEach(
        [&](DummyObjectWithNonzeroSize& obj)
        {
            iteratedObjects.insert(&obj);
        }
    );
    for (auto& obj : objects)
    {
        ASSERT_TRUE(iteratedObjects.contains(obj.get()));
    }
}

TEST_F(PoolTests, ForEach_GivenObjectsRegistered_CallsCallbackForEachObject)
{
    Pool<DummyObjectWithNonzeroSize> pool;

    auto obj0 = pool.MakeUnique();
    auto obj1 = pool.MakeUnique();
    auto obj2 = pool.MakeUnique();

    std::unordered_set<DummyObjectWithNonzeroSize*> iteratedObjects;

    pool.ForEach(
        [&](DummyObjectWithNonzeroSize& obj)
        {
            iteratedObjects.insert(&obj);
        }
    );

    ASSERT_EQ(iteratedObjects.size(), static_cast<size_t>(3));
    ASSERT_TRUE(iteratedObjects.contains(obj0.get()));
    ASSERT_TRUE(iteratedObjects.contains(obj1.get()));
    ASSERT_TRUE(iteratedObjects.contains(obj2.get()));
}

TEST_F(PoolTests, ForEach_GivenRegisteredObjectDestructed_DoesNotCallCallbackForIt)
{
    Pool<DummyObjectWithNonzeroSize> pool;

    auto obj0 = pool.MakeUnique();
    auto obj1 = pool.MakeUnique();
    auto obj2 = pool.MakeUnique();

    std::unordered_set<DummyObjectWithNonzeroSize*> iteratedObjects;

    obj1.reset();

    pool.ForEach(
        [&](DummyObjectWithNonzeroSize& obj)
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

TEST_F(PoolTests, ForEach_GivenRegisteredObjectDestructedDuringCallback_NextForEachDoesNotIncludeIt)
{
    Pool<DummyObjectWithNonzeroSize> pool;

    auto obj0 = pool.MakeUnique();
    auto obj1 = pool.MakeUnique();
    auto obj2 = pool.MakeUnique();

    pool.ForEach(
        [&](DummyObjectWithNonzeroSize& obj)
        {
            if(&obj == obj1.get())
            {
                obj1.reset();
            }
        }
    );

    std::unordered_set<DummyObjectWithNonzeroSize*> iteratedObjects;
    pool.ForEach(
        [&](DummyObjectWithNonzeroSize& obj)
        {
            iteratedObjects.insert(&obj);
        }
    );

    ASSERT_EQ(iteratedObjects.size(), static_cast<size_t>(2));
    ASSERT_TRUE(iteratedObjects.contains(obj0.get()));
    ASSERT_TRUE(iteratedObjects.contains(obj2.get()));
}

TEST_F(PoolTests, ForEach_GivenRegisteredObjectUniquePtrReset_DestructorIsCalled)
{
    Pool<DestructionTrackingObject> pool;

    bool destructorWasCalled = false;
    auto obj = pool.MakeUnique(destructorWasCalled);

    obj.reset();

    ASSERT_TRUE(destructorWasCalled);
}

TEST_F(PoolTests, ForEach_GivenRegisteredObjectLaterInIterationIsDestructedDuringCallback_DestructedObjectsAreNotReturned)
{
    Pool<DestructionTrackingObject> pool;

    std::array<bool, 3> objectsDestructed = { false, false, false };
    std::array<PoolUniquePtr<DestructionTrackingObject>::T, 3> objectsUniquePtr = {
        pool.MakeUnique(objectsDestructed[0]),
        pool.MakeUnique(objectsDestructed[1]),
        pool.MakeUnique(objectsDestructed[2])
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
    pool.ForEach(
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

TEST_F(PoolTests, ForEach_GivenNewObjectRegisteredDuringCallback_NextForEachDoesIncludeIt)
{
    Pool<DummyObjectWithNonzeroSize> pool;

    auto obj0 = pool.MakeUnique();
    auto obj1 = pool.MakeUnique();
    auto obj2 = pool.MakeUnique();

    PoolUniquePtr<DummyObjectWithNonzeroSize>::T obj3;
    pool.ForEach(
        [&](DummyObjectWithNonzeroSize&)
        {
            obj3 = pool.MakeUnique();
        }
    );

    std::unordered_set<DummyObjectWithNonzeroSize*> iteratedObjects;
    pool.ForEach(
        [&](DummyObjectWithNonzeroSize& obj)
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

TEST_F(PoolTests, ForEach_GivenCallbackCallsForEach_InnerAndOuterLoopOverSameObjects)
{
    Pool<DummyObjectWithNonzeroSize> pool;

    auto obj0 = pool.MakeUnique();
    auto obj1 = pool.MakeUnique();
    auto obj2 = pool.MakeUnique();

    std::vector<std::pair<DummyObjectWithNonzeroSize*, DummyObjectWithNonzeroSize*>> iteratedObjects;

    std::unique_ptr<DummyObjectWithNonzeroSize> obj3;
    pool.ForEach(
        [&](DummyObjectWithNonzeroSize& outerObj)
        {
            pool.ForEach(
                [&](DummyObjectWithNonzeroSize& innerObj)
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