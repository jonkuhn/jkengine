#include <csignal>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "JkEng/AfterBuildPtr.h"

using namespace testing;
using namespace JkEng;

class AfterBuildPtrTests : public Test
{
public:
    AfterBuildPtrTests()
    {

    }
};

class DummyObject
{
public:
    int a = 0;
    int b = 0;
};

TEST_F(AfterBuildPtrTests, Initialize_CalledTwice_Aborts)
{
    // Test is only relevant to debug builds because it tests for an assert
#if NDEBUG
    return;
#endif

    AfterBuildPtr<int> obj;
    int n = 123;
    obj.Initialize(&n);
    int m = 456;
    ASSERT_EXIT(obj.Initialize(&m), KilledBySignal(SIGABRT), "AfterBuildPtr: Initialize must only be called once");

}

TEST_F(AfterBuildPtrTests, Get_AfterInitialize_ReturnsPointerPassedToInitialize)
{
    AfterBuildPtr<int> obj;

    int n = 123;
    obj.Initialize(&n);

    ASSERT_EQ(obj.Get(), &n);
}

TEST_F(AfterBuildPtrTests, Get_WithoutInitialize_Aborts)
{
    // Test is only relevant to debug builds because it tests for an assert
#if NDEBUG
    return;
#endif

    AfterBuildPtr<int> obj;
    ASSERT_EXIT(obj.Get(), KilledBySignal(SIGABRT), "AfterBuildPtr: Get called before Initialize");

}

TEST_F(AfterBuildPtrTests, DereferenceOperator_AfterInitialize_ReturnsReferenceToObjectPassedToInitialize)
{
    AfterBuildPtr<int> obj;

    int n = 123;
    obj.Initialize(&n);

    ASSERT_EQ(&(*obj), &n);
}

TEST_F(AfterBuildPtrTests, DereferenceOperator_WithoutInitialize_Aborts)
{
    // Test is only relevant to debug builds because it tests for an assert
#if NDEBUG
    return;
#endif

    AfterBuildPtr<int> obj;
    ASSERT_EXIT( (*obj = 5), KilledBySignal(SIGABRT), "AfterBuildPtr: Get called before Initialize");

}

TEST_F(AfterBuildPtrTests, ArrowOperator_AfterInitialize_ReturnsPointerPassedToInitialize)
{
    AfterBuildPtr<DummyObject> obj;

    DummyObject dummy;
    obj.Initialize(&dummy);

    ASSERT_EQ(&(obj->b), &(dummy.b));
}

TEST_F(AfterBuildPtrTests, Arrow_WithoutInitialize_Aborts)
{
    // Test is only relevant to debug builds because it tests for an assert
#if NDEBUG
    return;
#endif

    AfterBuildPtr<DummyObject> obj;
    ASSERT_EXIT((obj->b = 5), KilledBySignal(SIGABRT), "AfterBuildPtr: Get called before Initialize");

}

