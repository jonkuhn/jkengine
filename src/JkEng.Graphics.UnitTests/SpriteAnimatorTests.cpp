#include <stdexcept>
#include <string>
#include <sstream>

#include <gtest/gtest.h>

#include <JkEng/Graphics/SpriteAnimator.h>
#include "MockSprite.h"
#include "TestHelpers.h"

using namespace testing;
using namespace JkEng::Graphics;

class SpriteAnimatorTests : public Test
{
public:
    SpriteAnimatorTests()
    {

    }

protected:
    NiceMock<MockSprite> _mockSprite;
};

TEST_F(SpriteAnimatorTests, Update_GivenPlayNotCalled_AtlasLocationCalledForFirstFrameOfFirstAnimation)
{
    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            0.1f
        ),
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 2),
                GridLocation(2, 2),
                GridLocation(3, 2),
            }),
            0.1f
        )
    });

    EXPECT_CALL(_mockSprite, AtlasLocation(GridLocation(1, 1))).Times(1);
    spriteAnimator.Update(_mockSprite, 0.11f);
    Mock::VerifyAndClear(&_mockSprite);
}

TEST_F(SpriteAnimatorTests, Update_GivenPlayWasCalledAndDeltaLessThanFrameTime_AtlasLocationCalledForFirstFrame)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        )
    });

    // Expect first frame of only animation
    EXPECT_CALL(_mockSprite, AtlasLocation(GridLocation(1, 1))).Times(1);

    spriteAnimator.Play(0, false, 1.0f);
    spriteAnimator.Update(_mockSprite, frameTime / 2);
    
    Mock::VerifyAndClear(&_mockSprite);
}

TEST_F(SpriteAnimatorTests, Update_GivenPlayWasCalledAndDeltaMoreThanFrameTime_AtlasLocationCalledForSecondFrame)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        )
    });

    // Expect 2nd frame of only animation
    EXPECT_CALL(_mockSprite, AtlasLocation(GridLocation(2, 1))).Times(1);

    spriteAnimator.Play(0, false, 1.0f);
    spriteAnimator.Update(_mockSprite, frameTime * 1.5f);
    
    Mock::VerifyAndClear(&_mockSprite);
}

TEST_F(SpriteAnimatorTests, Update_GivenPlaySecondAnimationAndDeltaLessThanFrameTime_AtlasLocationCalledForFirstFrameOfSecondAnimation)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        ),
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 2),
                GridLocation(2, 2),
                GridLocation(3, 2),
            }),
            frameTime
        )
    });

    // Expect first frame of second animation
    EXPECT_CALL(_mockSprite, AtlasLocation(GridLocation(1, 2))).Times(1);

    spriteAnimator.Play(1, false, 1.0f);
    spriteAnimator.Update(_mockSprite, frameTime / 2);
    
    Mock::VerifyAndClear(&_mockSprite);
}

TEST_F(SpriteAnimatorTests, Update_GivenPlaySecondAnimationAndDeltaMoreThanFrameTime_AtlasLocationCalledForSecondFrameOfSecondAnimation)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        ),
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 2),
                GridLocation(2, 2),
                GridLocation(3, 2),
            }),
            frameTime
        )
    });

    // Expect second frame of second animation
    EXPECT_CALL(_mockSprite, AtlasLocation(GridLocation(2, 2))).Times(1);

    spriteAnimator.Play(1, false, 1.0f);
    spriteAnimator.Update(_mockSprite, frameTime * 1.5f);
    
    Mock::VerifyAndClear(&_mockSprite);
}

TEST_F(SpriteAnimatorTests, Update_GivenPlayWithoutLoopUpdateCalledMultipleTimesWithDeltaGreaterThanAllFrames_AtlasLocationCalledForLastFrame)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        )
    });

    const bool noLoop = false;
    spriteAnimator.Play(0, noLoop, 1.0f);


    // Only 3 frames but called with more than frame time 4 times
    // Note: 2 calls would be enough to get to last frame.  If looping
    // was enabled this would wrap back around to second frame.
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);

    // Expect last frame of only animation when next update is called
    EXPECT_CALL(_mockSprite, AtlasLocation(GridLocation(3, 1))).Times(1);

    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    
    Mock::VerifyAndClear(&_mockSprite);
}

TEST_F(SpriteAnimatorTests, Update_GivenPlayWithLoopUpdateCalledWithDeltaEquivalentToTwoFramesPastEnd_AtlasLocationCalledForSecondFrame)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        )
    });

    const bool loop = true;
    spriteAnimator.Play(0, loop, 1.0f);

    // Only 3 frames but called with more than frame time 4 times
    // Note: 2 calls would be enough to get to last frame.  With looping
    // enabled this should wrap back around to second frame.
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);

    // Expect second frame of only animation when next update is called
    EXPECT_CALL(_mockSprite, AtlasLocation(GridLocation(2, 1))).Times(1);

    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    
    Mock::VerifyAndClear(&_mockSprite);
}

TEST_F(SpriteAnimatorTests, Update_GivenPlayWithSpeedMultiplierHalfSpeedAndUpdateCalledForJustOverTwoFrameTimes_AtlasLocationCalledForSecondFrame)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        )
    });

    const float halfSpeedMultiplier = 0.5f;
    spriteAnimator.Play(0, false, halfSpeedMultiplier);

    // Expect first frame of animation
    // (called with total just over 1 frame time running at half speed)
    EXPECT_CALL(_mockSprite, AtlasLocation(GridLocation(1, 1))).Times(1);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    Mock::VerifyAndClear(&_mockSprite);

    // Expect second frame of animation
    // (called with total just over 2 frame time running at half speed)
    EXPECT_CALL(_mockSprite, AtlasLocation(GridLocation(2, 1))).Times(1);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    Mock::VerifyAndClear(&_mockSprite);
}

TEST_F(SpriteAnimatorTests, Update_GivenPausedOnSecondFrameAndUpdateCalledForMoreThanFrameTime_AtlasLocationCalledForSecondFrame)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        ),
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 2),
                GridLocation(2, 2),
                GridLocation(3, 2),
            }),
            frameTime
        )
    });

    // NOTE: Using 2nd animation protects against animation index
    // being reset on resume or pause.
    spriteAnimator.Play(1, false, 1.0f);

    // This should put the animation at the second frame
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);

    spriteAnimator.Pause();

    // Stays on second frame because it is paused
    EXPECT_CALL(_mockSprite, AtlasLocation(GridLocation(2, 2))).Times(1);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    Mock::VerifyAndClear(&_mockSprite);

    // Still stays on second frame because it is paused
    EXPECT_CALL(_mockSprite, AtlasLocation(GridLocation(2, 2))).Times(1);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    Mock::VerifyAndClear(&_mockSprite);
}

TEST_F(SpriteAnimatorTests, Update_GivenResumeAfterPauseAndUpdateCalledWithMoreThanFrameTime_AtlasLocationCalledForFrameAfterPausedFrame)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        ),
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 2),
                GridLocation(2, 2),
                GridLocation(3, 2),
            }),
            frameTime
        )
    });

    // NOTE: Using 2nd animation protects against animation index
    // being reset on resume or pause.
    spriteAnimator.Play(1, false, 1.0f);

    // This should put the animation at the second frame
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);

    spriteAnimator.Pause();

    // Stays on second frame because it is paused
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);

    spriteAnimator.Resume();

    // Still stays on second frame because of small update time
    EXPECT_CALL(_mockSprite, AtlasLocation(GridLocation(3, 2))).Times(1);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    Mock::VerifyAndClear(&_mockSprite);
}

TEST_F(SpriteAnimatorTests, Update_GivenResumeAfterPauseAndUpdateCalledWithSmallAmount_AtlasLocationCalledForPausedFrame)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        ),
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 2),
                GridLocation(2, 2),
                GridLocation(3, 2),
            }),
            frameTime
        )
    });

    // NOTE: Using 2nd animation protects against animation index
    // being reset on resume or pause.
    spriteAnimator.Play(1, false, 1.0f);

    // This should put the animation at the second frame
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);

    spriteAnimator.Pause();

    // Stays on second frame because it is paused
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);

    spriteAnimator.Resume();

    // Still stays on second frame because of small update time
    EXPECT_CALL(_mockSprite, AtlasLocation(GridLocation(2, 2))).Times(1);
    spriteAnimator.Update(_mockSprite, frameTime * 0.01f);
    Mock::VerifyAndClear(&_mockSprite);
}

TEST_F(SpriteAnimatorTests, CurrentAnimationIndex_GivenPlayNotCalled_Returns0)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        ),
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 2),
                GridLocation(2, 2),
                GridLocation(3, 2),
            }),
            frameTime
        )
    });

    ASSERT_EQ(spriteAnimator.CurrentAnimationIndex(), 0u);
}

TEST_F(SpriteAnimatorTests, CurrentAnimationIndex_GivenPlayCalledForIndex1_Returns1)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        ),
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 2),
                GridLocation(2, 2),
                GridLocation(3, 2),
            }),
            frameTime
        )
    });

    spriteAnimator.Play(1, false, 1.0f);

    ASSERT_EQ(spriteAnimator.CurrentAnimationIndex(), 1u);
}

TEST_F(SpriteAnimatorTests, CurrentAnimationIndex_GivenPlayCalledForIndex1AndUpdateAndPauseCalled_Returns1)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        ),
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 2),
                GridLocation(2, 2),
                GridLocation(3, 2),
            }),
            frameTime
        )
    });

    spriteAnimator.Play(1, false, 1.0f);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    spriteAnimator.Pause();
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);

    ASSERT_EQ(spriteAnimator.CurrentAnimationIndex(), 1u);
}

TEST_F(SpriteAnimatorTests, CurrentAnimationIndex_GivenPlayCalledForIndex1AndUpdateAndPauseAndResumeCalled_Returns1)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        ),
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 2),
                GridLocation(2, 2),
                GridLocation(3, 2),
            }),
            frameTime
        )
    });

    spriteAnimator.Play(1, false, 1.0f);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    spriteAnimator.Pause();
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    spriteAnimator.Resume();

    ASSERT_EQ(spriteAnimator.CurrentAnimationIndex(), 1u);
}

TEST_F(SpriteAnimatorTests, CurrentAnimationIsFinished_GivenPlayNoLoopAndOnFirstFrame_ReturnsFalse)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        ),
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 2),
                GridLocation(2, 2),
                GridLocation(3, 2),
            }),
            frameTime
        )
    });

    // NOTE: Use animation at index 1 to guard against always checking
    // first animation
    const bool noLoop = false;
    spriteAnimator.Play(1, noLoop, 1.0f);
    
    EXPECT_FALSE(spriteAnimator.CurrentAnimationIsFinished());
}

TEST_F(SpriteAnimatorTests, CurrentAnimationIsFinished_GivenPlayNoLoopAndOnSecondFrame_ReturnsFalse)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        ),
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 2),
                GridLocation(2, 2),
                GridLocation(3, 2),
            }),
            frameTime
        )
    });

    // NOTE: Use animation at index 1 to guard against always checking
    // first animation
    const bool noLoop = false;
    spriteAnimator.Play(1, noLoop, 1.0f);

    // Should put animation on second frame
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    
    EXPECT_FALSE(spriteAnimator.CurrentAnimationIsFinished());
}

TEST_F(SpriteAnimatorTests, CurrentAnimationIsFinished_GivenPlayNoLoopAndOnLastFrame_ReturnsTrue)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        ),
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 2),
                GridLocation(2, 2),
                GridLocation(3, 2),
            }),
            frameTime
        )
    });

    // NOTE: Use animation at index 1 to guard against always checking
    // first animation
    const bool noLoop = false;
    spriteAnimator.Play(1, noLoop, 1.0f);

    // Should put animation on second frame
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);

    // Should put animation on third (last) frame
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    
    EXPECT_TRUE(spriteAnimator.CurrentAnimationIsFinished());
}

TEST_F(SpriteAnimatorTests, CurrentAnimationIsFinished_GivenPlayWithLoopAndOnLastFrame_ReturnsFalse)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        ),
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 2),
                GridLocation(2, 2),
                GridLocation(3, 2),
            }),
            frameTime
        )
    });

    // NOTE: Use animation at index 1 to guard against always checking
    // first animation
    const bool loop = true;
    spriteAnimator.Play(1, loop, 1.0f);

    // Should put animation on second frame
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);

    // Should put animation on third (last) frame
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    
    // With loop the animation is never finished so it should never
    // return true
    EXPECT_FALSE(spriteAnimator.CurrentAnimationIsFinished());
}

TEST_F(SpriteAnimatorTests, Update_GivenPlayCalledForAnimationDifferentThanCurrentThatWasPlayedPreviously_AnimationStartsAtFirstFrame)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        ),
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 2),
                GridLocation(2, 2),
                GridLocation(3, 2),
            }),
            frameTime
        )
    });

    const bool noLoop = false;

    // Get animation at index 1 to second frame
    spriteAnimator.Play(1, noLoop, 1.0f);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);

    // Get animation at index 0 to third frame
    spriteAnimator.Play(0, noLoop, 1.0f);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);

    // Play animation at index 1 (switching away from animation 0)
    spriteAnimator.Play(1, noLoop, 1.0f);

    // Should reset to first frame
    // Note: update is called with small delta time to stay on first frame
    EXPECT_CALL(_mockSprite, AtlasLocation(GridLocation(1, 2))).Times(1);
    spriteAnimator.Update(_mockSprite, frameTime * 0.01f);
    Mock::VerifyAndClear(&_mockSprite);
}

TEST_F(SpriteAnimatorTests, Update_GivenPlayCalledForCurrentAnimationWhileNotOnFirstFrame_AnimationDoesNotRestartFromFirstFrame)
{
    const float frameTime = 0.1f;

    SpriteAnimator spriteAnimator({
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 1),
                GridLocation(2, 1),
                GridLocation(3, 1),
            }),
            frameTime
        ),
        Animation(
            std::vector<GridLocation>({
                GridLocation(1, 2),
                GridLocation(2, 2),
                GridLocation(3, 2),
            }),
            frameTime
        )
    });

    const bool noLoop = false;

    // Get animation at index 1 to second frame
    spriteAnimator.Play(1, noLoop, 1.0f);
    spriteAnimator.Update(_mockSprite, frameTime * 1.1f);

    // Play animation at index 1 (not switching animations)
    spriteAnimator.Play(1, noLoop, 1.0f);

    // Should stay on second frame (reset is only triggered when animation changes)
    // Note: update is called with small delta time to stay on second frame
    EXPECT_CALL(_mockSprite, AtlasLocation(GridLocation(2, 2))).Times(1);
    spriteAnimator.Update(_mockSprite, frameTime * 0.01f);
    Mock::VerifyAndClear(&_mockSprite);
}