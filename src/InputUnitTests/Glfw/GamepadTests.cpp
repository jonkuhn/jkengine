#include <string>
#include <sstream>

#include <gtest/gtest.h>

#include <jkengine/Input/Glfw/Gamepad.h>
#include "MockGlfwInputWrapper.h"

using namespace testing;
using namespace Input::Glfw;

class GamepadTests : public Test
{
public:
    GamepadTests()
    {

    }

protected:
    NiceMock<MockGlfwInputWrapper> _mockGlfw;
};

TEST_F(GamepadTests, DisplayName_ReturnsJoystickIDPlusName)
{
    const int testJoystickId = 12;
    const char* testJoystickName = "the gamepad name";

    ON_CALL(_mockGlfw, GetJoystickName(testJoystickId))
        .WillByDefault(Return(testJoystickName));

    Gamepad gamepad(_mockGlfw, testJoystickId);

    EXPECT_EQ(gamepad.DisplayName(), "(" + std::to_string(testJoystickId) + ") " + testJoystickName);
}

TEST_F(GamepadTests, Update_GivenGetStateReturnsFalse_IsConnectedReturnsFalse)
{
    const int testJoystickId = 12;
    ON_CALL(_mockGlfw, GetGamepadState(testJoystickId, _))
        .WillByDefault(Return(GLFW_FALSE));

    Gamepad gamepad(_mockGlfw, testJoystickId);
    
    gamepad.Update();

    EXPECT_FALSE(gamepad.IsConnected());
}

TEST_F(GamepadTests, Update_GivenGetStateReturnsTrue_IsConnectedReturnsTrue)
{
    const int testJoystickId = 12;
    ON_CALL(_mockGlfw, GetGamepadState(testJoystickId, _))
        .WillByDefault(Return(GLFW_TRUE));

    Gamepad gamepad(_mockGlfw, testJoystickId);
    
    gamepad.Update();

    EXPECT_TRUE(gamepad.IsConnected());
}

TEST_F(GamepadTests, Update_GivenGetStateAxesValues_GettersReturnExpectedValues)
{
    const float testAxisLeftXValue = 0.0625f;
    const float testAxisLeftYValue = 0.125f;
    const float testAxisRightXValue = -0.25f;
    const float testAxisRightYValue = -0.5f;

    const int testJoystickId = 12;
    ON_CALL(_mockGlfw, GetGamepadState(testJoystickId, _))
        .WillByDefault(DoAll(
        [&](int, GLFWgamepadstate* state)
        {
            state->axes[GLFW_GAMEPAD_AXIS_LEFT_X] = testAxisLeftXValue;
            state->axes[GLFW_GAMEPAD_AXIS_LEFT_Y] = testAxisLeftYValue;
            state->axes[GLFW_GAMEPAD_AXIS_RIGHT_X] = testAxisRightXValue;
            state->axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] = testAxisRightYValue;
        },
        Return(GLFW_TRUE)));

    Gamepad gamepad(_mockGlfw, testJoystickId);
    
    gamepad.Update();

    EXPECT_EQ(gamepad.LeftStickX(), testAxisLeftXValue);
    EXPECT_EQ(gamepad.LeftStickY(), testAxisLeftYValue);
    EXPECT_EQ(gamepad.RightStickX(), testAxisRightXValue);
    EXPECT_EQ(gamepad.RightStickY(), testAxisRightYValue);
}

TEST_F(GamepadTests, Update_GivenEveryOtherButtonPressedEven_GettersReturnExpectedValues)
{
    // Note: For (hopefully) broader compatibility our Gamepad
    // class treats the triggers as buttons with the threshold
    // for activation set to 0.0f
    const float testAxisTriggerPressed = 0.0625f;
    const float testAxisTriggerReleased = -0.0625f;

    const int testJoystickId = 12;
    ON_CALL(_mockGlfw, GetGamepadState(testJoystickId, _))
        .WillByDefault(DoAll(
        [&](int, GLFWgamepadstate* state)
        {
            state->axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] = testAxisTriggerPressed;
            state->axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] = testAxisTriggerReleased;
            state->buttons[GLFW_GAMEPAD_BUTTON_A] = GLFW_PRESS;
            state->buttons[GLFW_GAMEPAD_BUTTON_B] = GLFW_RELEASE;
            state->buttons[GLFW_GAMEPAD_BUTTON_X] = GLFW_PRESS;
            state->buttons[GLFW_GAMEPAD_BUTTON_Y] = GLFW_RELEASE;
            state->buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] = GLFW_PRESS;
            state->buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] = GLFW_RELEASE;
            state->buttons[GLFW_GAMEPAD_BUTTON_BACK] = GLFW_PRESS;
            state->buttons[GLFW_GAMEPAD_BUTTON_START] = GLFW_RELEASE;
            state->buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB] = GLFW_PRESS;
            state->buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB] = GLFW_RELEASE;
            state->buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] = GLFW_PRESS;
            state->buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] = GLFW_RELEASE;
            state->buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] = GLFW_PRESS;
            state->buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] = GLFW_RELEASE;
        },
        Return(GLFW_TRUE)));

    Gamepad gamepad(_mockGlfw, testJoystickId);
    
    gamepad.Update();

    EXPECT_TRUE(gamepad.LeftTriggerButton());
    EXPECT_FALSE(gamepad.RightTriggerButton());
    EXPECT_TRUE(gamepad.BottomFaceButton());
    EXPECT_FALSE(gamepad.RightFaceButton());
    EXPECT_TRUE(gamepad.LeftFaceButton());
    EXPECT_FALSE(gamepad.TopFaceButton());
    EXPECT_TRUE(gamepad.LeftBumperButton());
    EXPECT_FALSE(gamepad.RightBumperButton());
    EXPECT_TRUE(gamepad.SelectButton());
    EXPECT_FALSE(gamepad.StartButton());
    EXPECT_TRUE(gamepad.LeftStickButton());
    EXPECT_FALSE(gamepad.RightStickButton());
    EXPECT_TRUE(gamepad.DPadUp());
    EXPECT_FALSE(gamepad.DPadRight());
    EXPECT_TRUE(gamepad.DPadDown());
    EXPECT_FALSE(gamepad.DPadLeft());
}

TEST_F(GamepadTests, Update_GivenEveryOtherButtonPressedOdd_GettersReturnExpectedValues)
{
    // Note: For (hopefully) broader compatibility our Gamepad
    // class treats the triggers as buttons with the threshold
    // for activation set to 0.0f
    const float testAxisTriggerPressed = 0.0625f;
    const float testAxisTriggerReleased = -0.0625f;

    const int testJoystickId = 12;
    ON_CALL(_mockGlfw, GetGamepadState(testJoystickId, _))
        .WillByDefault(DoAll(
        [&](int, GLFWgamepadstate* state)
        {
            state->axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] = testAxisTriggerReleased;
            state->axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] = testAxisTriggerPressed;
            state->buttons[GLFW_GAMEPAD_BUTTON_A] = GLFW_RELEASE;
            state->buttons[GLFW_GAMEPAD_BUTTON_B] = GLFW_PRESS;
            state->buttons[GLFW_GAMEPAD_BUTTON_X] = GLFW_RELEASE;
            state->buttons[GLFW_GAMEPAD_BUTTON_Y] = GLFW_PRESS;
            state->buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] = GLFW_RELEASE;
            state->buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] = GLFW_PRESS;
            state->buttons[GLFW_GAMEPAD_BUTTON_BACK] = GLFW_RELEASE;
            state->buttons[GLFW_GAMEPAD_BUTTON_START] = GLFW_PRESS;
            state->buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB] = GLFW_RELEASE;
            state->buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB] = GLFW_PRESS;
            state->buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] = GLFW_RELEASE;
            state->buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] = GLFW_PRESS;
            state->buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] = GLFW_RELEASE;
            state->buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] = GLFW_PRESS;
        },
        Return(GLFW_TRUE)));

    Gamepad gamepad(_mockGlfw, testJoystickId);
    
    gamepad.Update();

    EXPECT_FALSE(gamepad.LeftTriggerButton());
    EXPECT_TRUE(gamepad.RightTriggerButton());
    EXPECT_FALSE(gamepad.BottomFaceButton());
    EXPECT_TRUE(gamepad.RightFaceButton());
    EXPECT_FALSE(gamepad.LeftFaceButton());
    EXPECT_TRUE(gamepad.TopFaceButton());
    EXPECT_FALSE(gamepad.LeftBumperButton());
    EXPECT_TRUE(gamepad.RightBumperButton());
    EXPECT_FALSE(gamepad.SelectButton());
    EXPECT_TRUE(gamepad.StartButton());
    EXPECT_FALSE(gamepad.LeftStickButton());
    EXPECT_TRUE(gamepad.RightStickButton());
    EXPECT_FALSE(gamepad.DPadUp());
    EXPECT_TRUE(gamepad.DPadRight());
    EXPECT_FALSE(gamepad.DPadDown());
    EXPECT_TRUE(gamepad.DPadLeft());

}
