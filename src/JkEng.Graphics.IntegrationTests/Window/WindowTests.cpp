
#include <string>
#include <sstream>

#include <gtest/gtest.h>

#include <JkEng/Window/GlfwWindow.h>
#include <JkEng/Window/GlfwWrapper.h>

using namespace testing;
using namespace JkEng::Window;

namespace
{
    const int testWinWidth = 800;
    const int testWinHeight = 600;
    const std::string testWinTitle = "test title";
}

TEST(WindowTests, CreateUpdateGetKey_DoesNotThrow)
{
    GlfwWrapper glfw;

    EXPECT_NO_THROW(
        GlfwWindow window(glfw, testWinWidth, testWinHeight, testWinTitle);
        window.Update();
        window.GetKey(GLFW_KEY_ESCAPE);
        window.Update();
    );
}

TEST(WindowTests, WindowShouldCloseReturnsFalseUntilCloseIsCalledThenReturnsTrue)
{
    GlfwWrapper glfw;

    GlfwWindow window(glfw, testWinWidth, testWinHeight, testWinTitle);
    EXPECT_FALSE(window.WindowShouldClose());
    EXPECT_FALSE(window.WindowShouldClose());
    window.Close();
    EXPECT_TRUE(window.WindowShouldClose());
    EXPECT_TRUE(window.WindowShouldClose());
}

TEST(WindowTests, AllowsMultipleWindowsAndDifferentTimes)
{
    GlfwWrapper glfw;

    EXPECT_NO_THROW(
        {
            GlfwWindow window1(glfw, testWinWidth, testWinHeight, testWinTitle);
        });

    EXPECT_NO_THROW(
        {
            GlfwWindow window2(glfw, testWinWidth, testWinHeight, testWinTitle);
        });
}

TEST(WindowTests, DisallowsMultipleWindowsAtTheSameTime)
{
    // To simplify management of setting the current context
    // and since the vast majority of OpenGL applications
    // only need one window, the GlfwWindow class currently
    // only allows one instance at a time.
    GlfwWrapper glfw;

    EXPECT_THROW(
        {
            GlfwWindow window1(glfw, testWinWidth, testWinHeight, testWinTitle);
            GlfwWindow window2(glfw, testWinWidth, testWinHeight, testWinTitle);
        },
        std::logic_error);
}
