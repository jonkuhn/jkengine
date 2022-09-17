#include <chrono>
#include <cstdint>
#include <memory>
#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "jkengine/Shared/Pool.h"

using namespace testing;
using namespace Shared;

class SmallObject
{
public:
    SmallObject(int32_t a, int32_t b)
      : _a(a), _b(b)
    {

    }

    inline int32_t a() const { return _a; }
    inline int32_t b() const { return _b; }

private:
    int32_t _a;
    int32_t _b;
};


class PoolTests : public Test
{
public:
    PoolTests()
    {

    }
};

TEST_F(PoolTests, SmallObject_InsertionPerformance)
{
    const int32_t objectCount = 10000;

    {
        Pool<SmallObject, 128> pool;
        std::vector<PoolUniquePtr<SmallObject>::T> keepAlive;

        auto start = std::chrono::high_resolution_clock::now();

        for (int32_t i = 0; i < objectCount; i++)
        {
            keepAlive.push_back(pool.MakeUnique(i*2, i));
        }

        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Pool<SmallObject>: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us"
            << std::endl;
    }

    {
        std::vector<SmallObject> vec;

        auto start = std::chrono::high_resolution_clock::now();

        for (int32_t i = 0; i < objectCount; i++)
        {
            vec.emplace_back(i*2, i);
        }

        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "std::vector<SmallObject>: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us"
            << std::endl;
    }

    {
        std::vector<std::unique_ptr<SmallObject>> vec;

        auto start = std::chrono::high_resolution_clock::now();

        for (int32_t i = 0; i < objectCount; i++)
        {
            vec.emplace_back(std::make_unique<SmallObject>(i*2, i));
        }

        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "std::vector<std::unique_ptr<SmallObject>>: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us"
            << std::endl;
    }
}

TEST_F(PoolTests, SmallObject_IterationPerformance)
{
    const int32_t objectCount = 10000;

    int32_t aTotal[3] = { 0, 0, 0 };
    int32_t bTotal[3] = { 0, 0, 0 };

    {
        Pool<SmallObject,128> pool;
        std::vector<PoolUniquePtr<SmallObject>::T> keepAlive;

        for (int32_t i = 0; i < objectCount; i++)
        {
            keepAlive.push_back(pool.MakeUnique(i*2, i));
        }

        auto start = std::chrono::high_resolution_clock::now();

        pool.ForEach([&](SmallObject& so)
            {
                aTotal[0] += so.a();
                bTotal[0] += so.b();
            });

        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Pool<SmallObject>: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us"
            << std::endl;
    }

    {
        std::vector<SmallObject> vec;

        for (int32_t i = 0; i < objectCount; i++)
        {
            vec.emplace_back(i*2, i);
        }

        auto start = std::chrono::high_resolution_clock::now();
        
        for(SmallObject& so : vec)
        {
            aTotal[1] += so.a();
            bTotal[1] += so.b();
        }

        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "std::vector<SmallObject>: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us"
            << std::endl;
    }

    {
        std::vector<std::unique_ptr<SmallObject>> vec;

        for (int32_t i = 0; i < objectCount; i++)
        {
            vec.emplace_back(std::make_unique<SmallObject>(i*2, i));
        }

        auto start = std::chrono::high_resolution_clock::now();
        
        for(std::unique_ptr<SmallObject>& so : vec)
        {
            aTotal[2] += so->a();
            bTotal[2] += so->b();
        }

        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "std::vector<std::unique_ptr<SmallObject>>: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us"
            << std::endl;

        ASSERT_EQ(aTotal[0], aTotal[1]);
        ASSERT_EQ(aTotal[0], aTotal[1]);
        ASSERT_EQ(bTotal[1], bTotal[2]);
        ASSERT_EQ(bTotal[1], bTotal[2]);
    }
}
