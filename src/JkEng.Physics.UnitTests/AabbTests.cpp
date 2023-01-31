#include <string>
#include <sstream>

#include <gtest/gtest.h>

#include "Aabb.h"

using namespace testing;
using namespace JkEng::Physics;

class AabbTests : public Test
{
public:
    AabbTests()
    {

    }

protected:
    static void DoNothingCollisionHandler(const IReadOnlyAabb2d&)
    {

    }
};

struct TestObjectInfo
{
    TestObjectInfo() = default;

    TestObjectInfo(int i_, std::string s_)
      : i(i_),
        s(s_)
    {

    }

    int i;
    std::string s;
};

TEST_F(AabbTests, Constructor_GivenConstructedWithPositionAndSize_VerifyAllProperties)
{
    glm::vec2 position(50.0f, 25.0f);
    glm::vec2 size(5.0f, 10.0f);
    glm::vec2 velocity(1.0f, 2.0f);
    glm::vec2 acceleration(-0.5f, -0.2f);
    TestObjectInfo objectInfo(123, "test-string");
    Aabb a(position, size, velocity, acceleration,
        DoNothingCollisionHandler, objectInfo);

    EXPECT_EQ(a.LeftXMin(), position.x);
    EXPECT_EQ(a.RightXMax(), position.x + size.x);
    EXPECT_EQ(a.BottomYMin(), position.y);
    EXPECT_EQ(a.TopYMax(), position.y + size.y);
    EXPECT_EQ(a.Position(), position);
    EXPECT_EQ(a.PreviousPosition(), position);
    EXPECT_EQ(a.Size(), size);
    EXPECT_EQ(a.Velocity(), velocity);
    EXPECT_EQ(a.Acceleration(), acceleration);

    // Note: not comparing CollisionHandler() because std::function does not
    // implement operator== and comparing using target was more trouble than
    // it was worth.

    // Expect it to take a copy of object info, so pointers should not
    // be equal but contents should be equal
    EXPECT_NE(&(a.ObjectInfoAs<TestObjectInfo>()), &objectInfo);
    EXPECT_EQ(a.ObjectInfoAs<TestObjectInfo>().i, objectInfo.i);
    EXPECT_EQ(a.ObjectInfoAs<TestObjectInfo>().s, objectInfo.s);
}

TEST_F(AabbTests, Constructor_GivenConstructedWithXMinMaxYMinMax_VerifyAllProperties)
{
    float leftXMin = 50.0f;
    float rightXMax = 100.0f;
    float bottomYMin = 10.0f;
    float topYMax = 11.0f;
    glm::vec2 velocity(1.0f, 2.0f);
    glm::vec2 acceleration(-0.5f, -0.2f);
    TestObjectInfo objectInfo(123, "test-string");
    Aabb a(leftXMin, rightXMax, bottomYMin, topYMax,
        velocity, acceleration, DoNothingCollisionHandler, objectInfo);

    EXPECT_EQ(a.LeftXMin(), leftXMin);
    EXPECT_EQ(a.RightXMax(), rightXMax);
    EXPECT_EQ(a.BottomYMin(), bottomYMin);
    EXPECT_EQ(a.TopYMax(), topYMax);
    EXPECT_EQ(a.Position(), glm::vec2(leftXMin, bottomYMin));
    EXPECT_EQ(a.PreviousPosition(), glm::vec2(leftXMin, bottomYMin));
    EXPECT_EQ(a.Size(), glm::vec2(rightXMax - leftXMin, topYMax - bottomYMin));
    EXPECT_EQ(a.Velocity(), velocity);
    EXPECT_EQ(a.Acceleration(), acceleration);

    // Note: not comparing CollisionHandler() because std::function does not
    // implement operator== and comparing using target was more trouble than
    // it was worth.

    // Expect it to take a copy of object info, so pointers should not
    // be equal but contents should be equal
    EXPECT_NE(&(a.ObjectInfoAs<TestObjectInfo>()), &objectInfo);
    EXPECT_EQ(a.ObjectInfoAs<TestObjectInfo>().i, objectInfo.i);
    EXPECT_EQ(a.ObjectInfoAs<TestObjectInfo>().s, objectInfo.s);
}

TEST_F(AabbTests, Constructors_GivenEquivalentObjectsMadeWithDifferentConstructors_AllPropertiesAreEqual)
{
    float leftXMin = 50.0f;
    float rightXMax = 100.0f;
    float bottomYMin = 10.0f;
    float topYMax = 11.0f;
    glm::vec2 velocity(1.0f, 2.0f);
    glm::vec2 acceleration(-0.5f, -0.2f);
    Aabb a(leftXMin, rightXMax, bottomYMin, topYMax,
        velocity, acceleration, DoNothingCollisionHandler, TestObjectInfo());
    Aabb b(
        glm::vec2(leftXMin, bottomYMin),
        glm::vec2(rightXMax - leftXMin, topYMax - bottomYMin),
        velocity,
        acceleration,
        DoNothingCollisionHandler,
        TestObjectInfo());

    // Note: not comparing CollisionHandler() because std::function does not
    // implement operator== and comparing using target was more trouble than
    // it was worth.

    EXPECT_EQ(a.LeftXMin(), b.LeftXMin());
    EXPECT_EQ(a.RightXMax(), b.RightXMax());
    EXPECT_EQ(a.BottomYMin(), b.BottomYMin());
    EXPECT_EQ(a.TopYMax(), b.TopYMax());
    EXPECT_EQ(a.Position(), b.Position());
    EXPECT_EQ(a.Size(), b.Size());
    EXPECT_EQ(a.Velocity(), b.Velocity());
    EXPECT_EQ(a.Acceleration(), b.Acceleration());
}

TEST_F(AabbTests, IsColliding_XOverlapYNoOverlap_ReturnsFalse)
{
    // Use leftXMin, rightXMax, bottomYMin, topYMax constructor
    Aabb a(50.0f, 100.0f, 10.0f, 11.0f, glm::vec2(), glm::vec2(), DoNothingCollisionHandler, TestObjectInfo());
    Aabb b(99.9f, 150.0f, 9.0f, 9.9f, glm::vec2(), glm::vec2(), DoNothingCollisionHandler, TestObjectInfo());
    EXPECT_FALSE(a.IsColliding(b));
    EXPECT_FALSE(b.IsColliding(a));
}

TEST_F(AabbTests, IsColliding_XNoOverlapYOverlap_ReturnsFalse)
{
    // Use leftXMin, rightXMax, bottomYMin, topYMax constructor
    Aabb a(50.0f, 100.0f, 10.0f, 11.0f, glm::vec2(), glm::vec2(), DoNothingCollisionHandler, TestObjectInfo());
    Aabb b(100.1f, 150.0f, 9.0f, 10.1f, glm::vec2(), glm::vec2(), DoNothingCollisionHandler, TestObjectInfo());
    EXPECT_FALSE(a.IsColliding(b));
    EXPECT_FALSE(b.IsColliding(a));
}

TEST_F(AabbTests, IsColliding_XTouchingYOverlap_ReturnsTrue)
{
    // Use leftXMin, rightXMax, bottomYMin, topYMax constructor
    Aabb a(50.0f, 100.0f, 10.0f, 11.0f, glm::vec2(), glm::vec2(), DoNothingCollisionHandler, TestObjectInfo());
    Aabb b(100.0f, 150.0f, 9.0f, 10.1f, glm::vec2(), glm::vec2(), DoNothingCollisionHandler, TestObjectInfo());
    EXPECT_TRUE(a.IsColliding(b));
    EXPECT_TRUE(b.IsColliding(a));
}

TEST_F(AabbTests, IsColliding_XOverlapYTouching_ReturnsTrue)
{
    // Use leftXMin, rightXMax, bottomYMin, topYMax constructor
    Aabb a(50.0f, 100.0f, 10.0f, 11.0f, glm::vec2(), glm::vec2(), DoNothingCollisionHandler, TestObjectInfo());
    Aabb b(99.9f, 150.0f, 9.0f, 10.0f, glm::vec2(), glm::vec2(), DoNothingCollisionHandler, TestObjectInfo());
    EXPECT_TRUE(a.IsColliding(b));
    EXPECT_TRUE(b.IsColliding(a));
}

TEST_F(AabbTests, IsColliding_XOverlapYOverlap_ReturnsTrue)
{
    // Use leftXMin, rightXMax, bottomYMin, topYMax constructor
    Aabb a(50.0f, 100.0f, 10.0f, 11.0f, glm::vec2(), glm::vec2(), DoNothingCollisionHandler, TestObjectInfo());
    Aabb b(99.9f, 150.0f, 9.0f, 10.1f, glm::vec2(), glm::vec2(), DoNothingCollisionHandler, TestObjectInfo());
    EXPECT_TRUE(a.IsColliding(b));
    EXPECT_TRUE(b.IsColliding(a));
}

TEST_F(AabbTests, IsColliding_ACompletelyInsideB_ReturnsTrue)
{
    // Use leftXMin, rightXMax, bottomYMin, topYMax constructor
    Aabb a(75.0f, 80.0f, 60.0f, 65.0f, glm::vec2(), glm::vec2(), DoNothingCollisionHandler, TestObjectInfo());
    Aabb b(60.0f, 90.0f, 30.0f, 70.0f, glm::vec2(), glm::vec2(), DoNothingCollisionHandler, TestObjectInfo());
    EXPECT_TRUE(a.IsColliding(b));
    EXPECT_TRUE(b.IsColliding(a));
}

TEST_F(AabbTests, IsColliding_XOverlapYAligned_ReturnsTrue)
{
    // Use leftXMin, rightXMax, bottomYMin, topYMax constructor
    Aabb a(50.0f, 100.0f, 10.0f, 11.0f, glm::vec2(), glm::vec2(), DoNothingCollisionHandler, TestObjectInfo());
    Aabb b(0.0f, 150.1f, 10.0f, 11.0f, glm::vec2(), glm::vec2(), DoNothingCollisionHandler, TestObjectInfo());
    EXPECT_TRUE(a.IsColliding(b));
    EXPECT_TRUE(b.IsColliding(a));
}

TEST_F(AabbTests, IsColliding_XAlignedYOverlap_ReturnsTrue)
{
    // Use leftXMin, rightXMax, bottomYMin, topYMax constructor
    Aabb a(50.0f, 100.0f, 10.0f, 11.0f, glm::vec2(), glm::vec2(), DoNothingCollisionHandler, TestObjectInfo());
    Aabb b(50.0f, 100.0f, 10.9, 12.0f, glm::vec2(), glm::vec2(), DoNothingCollisionHandler, TestObjectInfo());
    EXPECT_TRUE(a.IsColliding(b));
    EXPECT_TRUE(b.IsColliding(a));
}

TEST_F(AabbTests, ObjectInfoAs_GivenCalledWithWrongType_ThrowsBadAnyCast)
{
    TestObjectInfo objectInfo(123, "test-string");
    Aabb a(glm::vec2(50.0f, 25.0f), glm::vec2(5.0f, 10.0f), glm::vec2(), glm::vec2(), DoNothingCollisionHandler, objectInfo);

    ASSERT_THROW(
        a.ObjectInfoAs<std::string>(),
        std::bad_any_cast
    );
}

TEST_F(AabbTests, ObjectInfoAs_GivenConstReference_ReturnsExpectedInfo)
{
    TestObjectInfo objectInfo(123, "test-string");
    Aabb a(glm::vec2(50.0f, 25.0f), glm::vec2(5.0f, 10.0f), glm::vec2(), glm::vec2(), DoNothingCollisionHandler, objectInfo);

    const Aabb& constRefA = a;

    auto& retrievedInfo = constRefA.ObjectInfoAs<TestObjectInfo>();

    EXPECT_EQ(retrievedInfo.i, objectInfo.i);
    EXPECT_EQ(retrievedInfo.s, objectInfo.s);
}

TEST_F(AabbTests, ObjectInfoAs_GivenModifiedViaReference_ConstObjectInfoAsReflectsChanges)
{
    TestObjectInfo objectInfo(123, "test-string");
    Aabb a(glm::vec2(50.0f, 25.0f), glm::vec2(5.0f, 10.0f), glm::vec2(), glm::vec2(), DoNothingCollisionHandler, objectInfo);
    const Aabb& constRefA = a;

    // modify one value via non-const ObjectInfoAs
    a.ObjectInfoAs<TestObjectInfo>().s = "modified";

    auto& retrievedInfo = constRefA.ObjectInfoAs<TestObjectInfo>();

    EXPECT_EQ(retrievedInfo.i, objectInfo.i);
    EXPECT_EQ(retrievedInfo.s, "modified");
}

TEST_F(AabbTests, Position_GivenNewPositionSet_OldPositionSavedInPreviousPosition)
{
    glm::vec2 initialPosition(50.0f, 25.0f);
    Aabb a(initialPosition, glm::vec2(5.0f, 10.0f), glm::vec2(), glm::vec2(), DoNothingCollisionHandler, nullptr);

    glm::vec2 newPosition1(51.0f, 26.0f);
    a.Position(newPosition1);

    // We need to change position twice since the Aabb is constructed
    // with previous position set to the same as the position passed
    // to the constructor
    glm::vec2 newPosition2(52.0f, 27.0f);
    a.Position(newPosition2);

    EXPECT_EQ(newPosition2, a.Position());
    EXPECT_EQ(newPosition1, a.PreviousPosition());
}
