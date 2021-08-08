#include <memory>
#include <string>
#include <sstream>

#include <gtest/gtest.h>

#include "Graphics/OpenGL/Window.h"
#include "Graphics/OpenGL/GlfwException.h"
#include "MockGlfwWrapper.h"
#include "../TestHelpers.h"

using namespace testing;
using namespace Graphics::OpenGL;

class WindowTests : public Test
{
public:
    WindowTests()
    {

    }

protected:
    NiceMock<MockGlfwWrapper> _mockGlfw;
    const int _testWinWidth = 123;
    const int _testWinHeight = 456;
    const std::string _testWinTitle = "test-win-title";
    GLFWwindow* const _testHandle = (GLFWwindow*)(0x7E577E57);

    void SetupMockCreateToAlwaysSucceed()
    {
        EXPECT_CALL(_mockGlfw, CreateWindow(_, _, _, _, _)).WillRepeatedly(Return(_testHandle));
    }

    void SetupMockLoadGlToAlwaysSucceed()
    {
        EXPECT_CALL(_mockGlfw, LoadGl()).WillRepeatedly(Return(true));
    }

};

TEST_F(WindowTests, Constructor_MakesRequiredCalls)
{
    // Note: Avoid making this test too brittle by not asserting
    // all of the window hints.
    EXPECT_CALL(_mockGlfw, CreateWindow(_testWinWidth, _testWinHeight, _testWinTitle.c_str(), nullptr, nullptr))
        .WillOnce(Return(_testHandle));
    EXPECT_CALL(_mockGlfw, MakeContextCurrent(_testHandle));
    EXPECT_CALL(_mockGlfw, SetFramebufferSizeCallback(_testHandle, _));
    EXPECT_CALL(_mockGlfw, LoadGl()).WillOnce(Return(true));

    GlfwWindow window(_mockGlfw, _testWinWidth, _testWinHeight, _testWinTitle);
}

TEST_F(WindowTests, Constructor_GivenCreateFails_Throws)
{
    EXPECT_CALL(_mockGlfw, CreateWindow(_, _, _, _, _))
        .WillOnce(Return(nullptr));

    EXPECT_THROW(
        GlfwWindow window(_mockGlfw, _testWinWidth, _testWinHeight, _testWinTitle),
        GlfwException);
}

TEST_F(WindowTests, Constructor_GivenLoadGlFails_ThrowsAndDestroysWindow)
{
    SetupMockCreateToAlwaysSucceed();
    EXPECT_CALL(_mockGlfw, LoadGl()).WillOnce(Return(false));
    EXPECT_CALL(_mockGlfw, DestroyWindow(_testHandle));

    EXPECT_THROW(
        GlfwWindow window(_mockGlfw, _testWinWidth, _testWinHeight, _testWinTitle),
        std::runtime_error);
}

TEST_F(WindowTests, GivenConstructAfterPreviousWindowIsDestroyed_Succeeds)
{
    // To simplify management of setting the current context
    // and since the vast majority of OpenGL applications
    // only need one window, the GlfwWindow class currently
    // only allows one instance at a time.
    SetupMockCreateToAlwaysSucceed();
    SetupMockLoadGlToAlwaysSucceed();
    {
        GlfwWindow window(_mockGlfw, _testWinWidth, _testWinHeight, _testWinTitle);
    }
    {
        GlfwWindow window(_mockGlfw, _testWinWidth, _testWinHeight, _testWinTitle);
    }
}

TEST_F(WindowTests, GivenConstructWhileOtherWindowExists_Throws)
{
    // To simplify management of setting the current context
    // and since the vast majority of OpenGL applications
    // only need one window, the GlfwWindow class currently
    // only allows one instance at a time.
    SetupMockCreateToAlwaysSucceed();
    SetupMockLoadGlToAlwaysSucceed();
    EXPECT_THROW(
        {
            GlfwWindow window1(_mockGlfw, _testWinWidth, _testWinHeight, _testWinTitle);
            GlfwWindow window2(_mockGlfw, _testWinWidth, _testWinHeight, _testWinTitle);
        },
        std::logic_error);
}

TEST_F(WindowTests, Destructor_CallsDestroyWindow)
{
    SetupMockCreateToAlwaysSucceed();
    SetupMockLoadGlToAlwaysSucceed();

    EXPECT_CALL(_mockGlfw, DestroyWindow(_testHandle)).Times(1);
    {
        GlfwWindow window(_mockGlfw, _testWinWidth, _testWinHeight, _testWinTitle);
    }
}

TEST_F(WindowTests, Close_CallsSetWindowShouldCloseWithTrue)
{
    SetupMockCreateToAlwaysSucceed();
    SetupMockLoadGlToAlwaysSucceed();
    GlfwWindow window(_mockGlfw, _testWinWidth, _testWinHeight, _testWinTitle);

    EXPECT_CALL(_mockGlfw, SetWindowShouldClose(_testHandle, true));
    window.Close();
}

TEST_F(WindowTests, GetKey_PassthroughToGlfwGetKey)
{
    const int testKey = 1111;
    const int testGetKeyReturn = 2222;
    SetupMockCreateToAlwaysSucceed();
    SetupMockLoadGlToAlwaysSucceed();
    GlfwWindow window(_mockGlfw, _testWinWidth, _testWinHeight, _testWinTitle);

    EXPECT_CALL(_mockGlfw, GetKey(_testHandle, testKey)).WillOnce(Return(testGetKeyReturn));
    EXPECT_EQ(window.GetKey(testKey), testGetKeyReturn);
}

TEST_F(WindowTests, Update_MakesExpectedCalls)
{
    SetupMockCreateToAlwaysSucceed();
    SetupMockLoadGlToAlwaysSucceed();
    GlfwWindow window(_mockGlfw, _testWinWidth, _testWinHeight, _testWinTitle);

    EXPECT_CALL(_mockGlfw, SwapBuffers(_testHandle));
    EXPECT_CALL(_mockGlfw, PollEvents());
    EXPECT_CALL(_mockGlfw, WindowShouldClose(_testHandle)).WillOnce(Return(false));
    window.Update();
}

TEST_F(WindowTests, Update_GivenWindowShouldNotClose_ReturnsTrue)
{
    SetupMockCreateToAlwaysSucceed();
    SetupMockLoadGlToAlwaysSucceed();
    GlfwWindow window(_mockGlfw, _testWinWidth, _testWinHeight, _testWinTitle);

    EXPECT_CALL(_mockGlfw, WindowShouldClose(_testHandle)).WillOnce(Return(false));
    EXPECT_TRUE(window.Update());
}

TEST_F(WindowTests, Update_GivenWindowShouldClose_ReturnsFalse)
{
    SetupMockCreateToAlwaysSucceed();
    SetupMockLoadGlToAlwaysSucceed();
    GlfwWindow window(_mockGlfw, _testWinWidth, _testWinHeight, _testWinTitle);

    EXPECT_CALL(_mockGlfw, WindowShouldClose(_testHandle)).WillOnce(Return(true));
    EXPECT_FALSE(window.Update());
}

TEST_F(WindowTests, FramebufferCallback_MakesExpectedCalls)
{
    SetupMockCreateToAlwaysSucceed();
    SetupMockLoadGlToAlwaysSucceed();

    // Use mock GLFW to capture the callback
    GLFWframebuffersizefun savedCallback = nullptr;
    EXPECT_CALL(_mockGlfw, SetFramebufferSizeCallback(_, _))
        .WillOnce(DoAll(SaveArg<1>(&savedCallback), Return(nullptr)));
    GlfwWindow window(_mockGlfw, _testWinWidth, _testWinHeight, _testWinTitle);

    // Setup expectations for what the callback should call
    int newWidth = _testWinWidth / 2;
    int newHeight = _testWinHeight / 2;
    EXPECT_CALL(_mockGlfw, MakeContextCurrent(_testHandle));
    EXPECT_CALL(_mockGlfw, SetGlViewport(0, 0, newWidth, newHeight));

    // Call the callback
    savedCallback(_testHandle, newWidth, newHeight);
}
