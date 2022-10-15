#include <csignal>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "JkEng/AfterCreatePtr.h"

using namespace testing;
using namespace JkEng;

class AfterCreatePtrTests : public Test
{
public:
    AfterCreatePtrTests()
    {

    }
};

class DummyObject
{
public:
    int a = 0;
    int b = 0;
};

TEST_F(AfterCreatePtrTests, Initialize_CalledTwice_Aborts)
{
    // Test is only relevant to debug builds because it tests for an assert
#if NDEBUG
    return;
#endif

    AfterCreatePtr<int> obj;
    int n = 123;
    obj.Initialize(&n);
    int m = 456;
    ASSERT_EXIT(obj.Initialize(&m), KilledBySignal(SIGABRT), "AfterCreatePtr: Initialize must only be called once");

}

TEST_F(AfterCreatePtrTests, Get_AfterInitialize_ReturnsPointerPassedToInitialize)
{
    AfterCreatePtr<int> obj;

    int n = 123;
    obj.Initialize(&n);

    ASSERT_EQ(obj.Get(), &n);
}

TEST_F(AfterCreatePtrTests, Get_WithoutInitialize_Aborts)
{
    // Test is only relevant to debug builds because it tests for an assert
#if NDEBUG
    return;
#endif

    AfterCreatePtr<int> obj;
    ASSERT_EXIT(obj.Get(), KilledBySignal(SIGABRT), "AfterCreatePtr: Get called before Initialize");

}

TEST_F(AfterCreatePtrTests, DereferenceOperator_AfterInitialize_ReturnsReferenceToObjectPassedToInitialize)
{
    AfterCreatePtr<int> obj;

    int n = 123;
    obj.Initialize(&n);

    ASSERT_EQ(&(*obj), &n);
}

TEST_F(AfterCreatePtrTests, DereferenceOperator_WithoutInitialize_Aborts)
{
    // Test is only relevant to debug builds because it tests for an assert
#if NDEBUG
    return;
#endif

    AfterCreatePtr<int> obj;
    ASSERT_EXIT( (*obj = 5), KilledBySignal(SIGABRT), "AfterCreatePtr: Get called before Initialize");

}

TEST_F(AfterCreatePtrTests, ArrowOperator_AfterInitialize_ReturnsPointerPassedToInitialize)
{
    AfterCreatePtr<DummyObject> obj;

    DummyObject dummy;
    obj.Initialize(&dummy);

    ASSERT_EQ(&(obj->b), &(dummy.b));
}

TEST_F(AfterCreatePtrTests, Arrow_WithoutInitialize_Aborts)
{
    // Test is only relevant to debug builds because it tests for an assert
#if NDEBUG
    return;
#endif

    AfterCreatePtr<DummyObject> obj;
    ASSERT_EXIT((obj->b = 5), KilledBySignal(SIGABRT), "AfterCreatePtr: Get called before Initialize");

}

