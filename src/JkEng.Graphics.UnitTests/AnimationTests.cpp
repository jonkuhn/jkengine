#include <stdexcept>
#include <string>
#include <sstream>

#include <gtest/gtest.h>

#include <JkEng/Graphics/Animation.h>
#include "MockSprite.h"
#include "TestHelpers.h"

using namespace testing;
using namespace JkEng::Graphics;

class AnimationTests : public Test
{
public:
    AnimationTests()
    {

    }
};

TEST_F(AnimationTests, Constructor_GivenEmptyFrames_ThrowsInvalidArgument)
{
    EXPECT_THROW(
        try
        {
            Animation a(std::vector<GridLocation>(), 0.1f);

        }
        catch (std::invalid_argument& e)
        {
            EXPECT_THAT(e.what(), HasSubstr("frames must not be empty"));
            throw;
        },
        std::invalid_argument);
}

TEST_F(AnimationTests, Constructor_GivenZeroDuration_ThrowsInvalidArgument)
{
    EXPECT_THROW(
        try
        {
            Animation a(std::vector<GridLocation>({GridLocation(1, 2)}), 0.0f);

        }
        catch (std::invalid_argument& e)
        {
            EXPECT_THAT(e.what(), HasSubstr("eachFrameDuration must be positive"));
            throw;
        },
        std::invalid_argument);
}

TEST_F(AnimationTests, Constructor_GivenNegativeDuration_ThrowsInvalidArgument)
{
    EXPECT_THROW(
        try
        {
            Animation a(std::vector<GridLocation>({GridLocation(1, 2)}), -0.1f);

        }
        catch (std::invalid_argument& e)
        {
            EXPECT_THAT(e.what(), HasSubstr("eachFrameDuration must be positive"));
            throw;
        },
        std::invalid_argument);
}

TEST_F(AnimationTests, CurrentAtlasLocation_GivenConstructedWithThreeFrames_ReturnsFirstFrameLocation)
{
    Animation a(
        std::vector<GridLocation>({
            GridLocation(1, 1),
            GridLocation(2, 2),
            GridLocation(3, 3),
        }),
        0.1f);

    auto current = a.CurrentAtlasLocation();
    EXPECT_EQ(current.x, 1u);
    EXPECT_EQ(current.y, 1u);
}

TEST_F(AnimationTests, Update_GivenCalledWithDeltaMoreThanFrameDuration_CurrentAtlasLocationReturnsSecondFrameLocation)
{
    Animation a(
        std::vector<GridLocation>({
            GridLocation(1, 1),
            GridLocation(2, 2),
            GridLocation(3, 3),
        }),
        0.1f);

    a.Update(0.15f, false, 1.0f);

    auto current = a.CurrentAtlasLocation();
    EXPECT_EQ(current.x, 2u);
    EXPECT_EQ(current.y, 2u);
}

TEST_F(AnimationTests, Update_GivenCalledWithDeltaLessThanFrameDuration_CurrentAtlasLocationReturnsSecondFrameLocation)
{
    Animation a(
        std::vector<GridLocation>({
            GridLocation(1, 1),
            GridLocation(2, 2),
            GridLocation(3, 3),
        }),
        0.1f);

    a.Update(0.05f, false, 1.0f);

    auto current = a.CurrentAtlasLocation();
    EXPECT_EQ(current.x, 1u);
    EXPECT_EQ(current.y, 1u);
}

TEST_F(AnimationTests, Update_GivenCalledMultipleTimesWithSumOfDeltasGreaterThanFrameDuration_CurrentAtlasLocationReturnsSecondFrameLocation)
{
    Animation a(
        std::vector<GridLocation>({
            GridLocation(1, 1),
            GridLocation(2, 2),
            GridLocation(3, 3),
        }),
        0.1f);

    a.Update(0.03f, false, 1.0f);
    a.Update(0.03f, false, 1.0f);
    a.Update(0.03f, false, 1.0f);
    a.Update(0.03f, false, 1.0f);

    auto current = a.CurrentAtlasLocation();
    EXPECT_EQ(current.x, 2u);
    EXPECT_EQ(current.y, 2u);
}

TEST_F(AnimationTests, Update_GivenCalledWithMoreThanFrameDurationThenLessThanFrameDuration_CurrentAtlasLocationReturnsSecondFrameLocation)
{
    // This tests that the frame time accumulator gets reset

    Animation a(
        std::vector<GridLocation>({
            GridLocation(1, 1),
            GridLocation(2, 2),
            GridLocation(3, 3),
        }),
        0.1f);

    // This triggers step from first to second frame
    a.Update(0.11f, false, 1.0f);

    // This should not trigger step from second to third frame
    a.Update(0.03f, false, 1.0f);

    auto current = a.CurrentAtlasLocation();
    EXPECT_EQ(current.x, 2u);
    EXPECT_EQ(current.y, 2u);
}

TEST_F(AnimationTests, Update_GivenNoLoopAndOnLastFrameAndUpdateCalledWithMoreThanFrameTime_CurrentAtlasLocationReturnsLastFrame)
{
    const bool noLoop = false;

    Animation a(
        std::vector<GridLocation>({
            GridLocation(1, 1),
            GridLocation(2, 2),
            GridLocation(3, 3),
        }),
        0.1f);

    // Steps from first to second, then second to third frame
    a.Update(0.11f, noLoop, 1.0f);
    a.Update(0.11f, noLoop, 1.0f);

    // Should not step past 3rd frame
    a.Update(0.11f, noLoop, 1.0f);
    a.Update(0.11f, noLoop, 1.0f);
    a.Update(0.11f, noLoop, 1.0f);

    auto current = a.CurrentAtlasLocation();
    EXPECT_EQ(current.x, 3u);
    EXPECT_EQ(current.y, 3u);
}

TEST_F(AnimationTests, Update_GivenLoopAndOnLastFrameAndUpdateCalledWithMoreThanFrameTime_CurrentAtlasLocationReturnsLastFrame)
{
    const bool loop = true;

    Animation a(
        std::vector<GridLocation>({
            GridLocation(1, 1),
            GridLocation(2, 2),
            GridLocation(3, 3),
        }),
        0.1f);

    // Steps from first to second, then second to third frame
    a.Update(0.11f, loop, 1.0f);
    a.Update(0.11f, loop, 1.0f);

    // Should wrap around to first frame
    a.Update(0.11f, loop, 1.0f);

    auto current = a.CurrentAtlasLocation();
    EXPECT_EQ(current.x, 1u);
    EXPECT_EQ(current.y, 1u);
}

TEST_F(AnimationTests, Update_GivenSpeedMultiplierLessThan1AndUpdateCalledWithJustLessThanSpeedMultiplierAdjustedFrameTime_CurrentAtlasLocationReturnsFirstFrame)
{
    const float speedMultiplier = 0.5f;

    Animation a(
        std::vector<GridLocation>({
            GridLocation(1, 1),
            GridLocation(2, 2),
            GridLocation(3, 3),
        }),
        0.1f);

    a.Update(0.19f, false, speedMultiplier);

    auto current = a.CurrentAtlasLocation();
    EXPECT_EQ(current.x, 1u);
    EXPECT_EQ(current.y, 1u);
}

TEST_F(AnimationTests, Update_GivenSpeedMultiplierMoreThan1AndUpdateCalledWithJustMoreThanSpeedMultiplierAdjustedFrameTime_CurrentAtlasLocationReturnsSecondFrame)
{
    const float speedMultiplier = 2.0f;

    Animation a(
        std::vector<GridLocation>({
            GridLocation(1, 1),
            GridLocation(2, 2),
            GridLocation(3, 3),
        }),
        0.1f);

    a.Update(0.06, false, speedMultiplier);

    auto current = a.CurrentAtlasLocation();
    EXPECT_EQ(current.x, 2u);
    EXPECT_EQ(current.y, 2u);
}

TEST_F(AnimationTests, IsOnLastFrame_GivenOnFirstFrame_ReturnsFalse)
{
    Animation a(
        std::vector<GridLocation>({
            GridLocation(1, 1),
            GridLocation(2, 2),
            GridLocation(3, 3),
        }),
        0.1f);

    EXPECT_EQ(a.IsOnLastFrame(), false);
}

TEST_F(AnimationTests, IsOnLastFrame_GivenOnSecondFrame_ReturnsFalse)
{
    Animation a(
        std::vector<GridLocation>({
            GridLocation(1, 1),
            GridLocation(2, 2),
            GridLocation(3, 3),
        }),
        0.1f);

    a.Update(0.11, false, 1.0f);

    EXPECT_EQ(a.IsOnLastFrame(), false);
}

TEST_F(AnimationTests, IsOnLastFrame_GivenOnLastFrame_ReturnsFalse)
{
    Animation a(
        std::vector<GridLocation>({
            GridLocation(1, 1),
            GridLocation(2, 2),
            GridLocation(3, 3),
        }),
        0.1f);

    a.Update(0.11, false, 1.0f);
    a.Update(0.11, false, 1.0f);

    EXPECT_EQ(a.IsOnLastFrame(), true);
}

TEST_F(AnimationTests, Reset_GivenWasNotOnFirstFrame_CurrentAtlasLocationReturnsFirstFrame)
{
    Animation a(
        std::vector<GridLocation>({
            GridLocation(1, 1),
            GridLocation(2, 2),
            GridLocation(3, 3),
        }),
        0.1f);

    a.Update(0.11, false, 1.0f);
    a.Reset();

    EXPECT_EQ(a.CurrentAtlasLocation().x, 1u);
    EXPECT_EQ(a.CurrentAtlasLocation().y, 1u);
}

TEST_F(AnimationTests, Reset_GivenWasAlmostReadyForNextFrameUpdateCalledWithDifferenceAfterReset_CurrentAtlasLocationReturnsFirstFrame)
{
    Animation a(
        std::vector<GridLocation>({
            GridLocation(1, 1),
            GridLocation(2, 2),
            GridLocation(3, 3),
        }),
        0.1f);

    a.Update(0.09, false, 1.0f);
    a.Reset();

    // If accumulator is not cleared this would advance to the next frame
    a.Update(0.02, false, 1.0f);

    EXPECT_EQ(a.CurrentAtlasLocation().x, 1u);
    EXPECT_EQ(a.CurrentAtlasLocation().y, 1u);
}


