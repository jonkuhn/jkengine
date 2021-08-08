
#include <string>
#include <sstream>

#include <gtest/gtest.h>

#include "Graphics/OpenGL/Window.h"
#include "Graphics/OpenGL/GlfwWrapper.h"
#include "TestHelpers.h"

using namespace testing;
using namespace Graphics::OpenGL;

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

TEST(WindowTests, UpdateReturnsTrueUntilCloseIsCalledThenReturnsFalse)
{
    GlfwWrapper glfw;

    GlfwWindow window(glfw, testWinWidth, testWinHeight, testWinTitle);
    EXPECT_TRUE(window.Update());
    EXPECT_TRUE(window.Update());
    window.Close();
    EXPECT_FALSE(window.Update());
    EXPECT_FALSE(window.Update());
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
