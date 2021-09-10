#include <string>
#include <sstream>

#include <gtest/gtest.h>

#include "Input/Glfw/Engine.h"
#include "FakeGlfwInputWindow.h"
#include "MockGlfwInputWrapper.h"

using namespace testing;
using namespace Input::Glfw;

class EngineTests : public Test
{
public:
    EngineTests()
    {

    }

protected:
    NiceMock<MockGlfwInputWrapper> _mockGlfw;
    FakeGlfwInputWindow _fakeInputWindow;
};

TEST_F(EngineTests, Update_CallsUpdateForAllJoystickIds)
{
    // Verify that update is called by checking that the underlying
    // GetGamepadState call is made.  This makes the test depend
    // on an implementation detail of Gamepad, but in this case
    // it seems like a good tradeoff versus going to the trouble
    // of fully decoupling Gamepad from Input::Glfw::Engine.
    for(int joystickId = GLFW_JOYSTICK_1; joystickId <= GLFW_JOYSTICK_LAST; joystickId++)
    {
        EXPECT_CALL(_mockGlfw, GetGamepadState(joystickId, _))
            .Times(1)
            .WillOnce(Return(GLFW_FALSE));
    }

    Engine engine(_mockGlfw, _fakeInputWindow);
    engine.Update();
}

TEST_F(EngineTests, Update_GivenFourGamepadsConnected_EnumerateGamepadsReturnsFourNonNullGamepads)
{
    ON_CALL(_mockGlfw, GetGamepadState(GLFW_JOYSTICK_1, _)).WillByDefault(Return(GLFW_TRUE));
    ON_CALL(_mockGlfw, GetGamepadState(GLFW_JOYSTICK_2, _)).WillByDefault(Return(GLFW_FALSE));
    ON_CALL(_mockGlfw, GetGamepadState(GLFW_JOYSTICK_3, _)).WillByDefault(Return(GLFW_TRUE));
    ON_CALL(_mockGlfw, GetGamepadState(GLFW_JOYSTICK_4, _)).WillByDefault(Return(GLFW_FALSE));
    ON_CALL(_mockGlfw, GetGamepadState(GLFW_JOYSTICK_5, _)).WillByDefault(Return(GLFW_TRUE));
    ON_CALL(_mockGlfw, GetGamepadState(GLFW_JOYSTICK_6, _)).WillByDefault(Return(GLFW_FALSE));
    ON_CALL(_mockGlfw, GetGamepadState(GLFW_JOYSTICK_7, _)).WillByDefault(Return(GLFW_TRUE));
    ON_CALL(_mockGlfw, GetGamepadState(GLFW_JOYSTICK_8, _)).WillByDefault(Return(GLFW_FALSE));
    ON_CALL(_mockGlfw, GetGamepadState(GLFW_JOYSTICK_9, _)).WillByDefault(Return(GLFW_FALSE));
    ON_CALL(_mockGlfw, GetGamepadState(GLFW_JOYSTICK_10, _)).WillByDefault(Return(GLFW_FALSE));
    ON_CALL(_mockGlfw, GetGamepadState(GLFW_JOYSTICK_11, _)).WillByDefault(Return(GLFW_FALSE));
    ON_CALL(_mockGlfw, GetGamepadState(GLFW_JOYSTICK_12, _)).WillByDefault(Return(GLFW_FALSE));
    ON_CALL(_mockGlfw, GetGamepadState(GLFW_JOYSTICK_13, _)).WillByDefault(Return(GLFW_FALSE));
    ON_CALL(_mockGlfw, GetGamepadState(GLFW_JOYSTICK_14, _)).WillByDefault(Return(GLFW_FALSE));
    ON_CALL(_mockGlfw, GetGamepadState(GLFW_JOYSTICK_15, _)).WillByDefault(Return(GLFW_FALSE));
    ON_CALL(_mockGlfw, GetGamepadState(GLFW_JOYSTICK_16, _)).WillByDefault(Return(GLFW_FALSE));

    Engine engine(_mockGlfw, _fakeInputWindow);
    engine.Update();

    auto connectedGamepads = engine.EnumerateGamepads();
    EXPECT_EQ(connectedGamepads.size(), 4U);
    for (auto* gamepad : connectedGamepads)
    {
        EXPECT_NE(gamepad, nullptr);
    }
}

TEST_F(EngineTests, EnumerateGamepads_GivenCalledBeforeUpdate_Throws)
{
    Engine engine(_mockGlfw, _fakeInputWindow);
    EXPECT_THROW(engine.EnumerateGamepads(), std::logic_error);
}
