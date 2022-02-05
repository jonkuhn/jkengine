#include "GlfwWindow.h"
#include "GlfwException.h"

using namespace Window;

GlfwWindow* GlfwWindow::s_singleInstance = nullptr;

GlfwWindow::GlfwWindow(IGlfwWrapper& glfw, int winWidth, int winHeight, const std::string& title)
    : _glfw(glfw)
{
    // Many of the GLFW calls below must only be called from the main
    // thread anyway, so there is not much sense in making this check
    // thread safe.
    if (s_singleInstance != nullptr)
    {
        // To allow for multiple windows we would need to be able to
        // make sure _glfw->MakeContextCurrent is called for the appropriate
        // window before making OpenGL calls for that window.
        throw std::logic_error(
            "To simplify context management, the GlfwWindow class currently "
            "only supports one instance at a time.");
    }

    _glfw.WindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    _glfw.WindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    _glfw.WindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    _glfw.WindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    _handle = UniqueWindowHandle(
        _glfw.CreateWindow(winWidth, winHeight, title.c_str(), nullptr, nullptr),
        [this](GLFWwindow* h){ _glfw.DestroyWindow(h);});
    if (_handle == nullptr)
    {
        throw GlfwException(_glfw, "Failed to create GLFW window.");
    }
    _glfw.MakeContextCurrent(_handle.get());
    _glfw.SetFramebufferSizeCallback(_handle.get(), FrameBufferSizeCallbackDispatch);

    if (!_glfw.LoadGl())
    {
        throw std::runtime_error("Failed to initialize OpenGL using GLFW and GLAD");
    } 
    s_singleInstance = this;
}

GlfwWindow::~GlfwWindow()
{
    _handle.reset();
    s_singleInstance = nullptr;
}

void GlfwWindow::Close()
{
    _glfw.SetWindowShouldClose(_handle.get(), true);
}

int GlfwWindow::GetKey(int key)
{
    return _glfw.GetKey(_handle.get(), key);
}

void GlfwWindow::Update()
{
    _glfw.SwapBuffers(_handle.get());
    _glfw.PollEvents();
}

bool GlfwWindow::WindowShouldClose()
{
    return _glfw.WindowShouldClose(_handle.get());
}

void GlfwWindow::FrameBufferSizeCallbackDispatch(GLFWwindow* window, int width, int height)
{
    // For now keep dispatch simple since we are only allowing
    // one instance of GlfwWindow at a time.  If we ever want multiple
    // instances we should instead use glfwSetWindowUserPointer/glfwGetWindowUserPointer
    // but that will also make it so this static function depends on GLFW
    // which we wanted to be able to replace for testing.
    s_singleInstance->FrameBufferSizeCallback(window, width, height);
}

void GlfwWindow::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Make sure the right context is current, even though
    // currently this class is limited to one instance at a
    // time. (see comment in constructor)
    _glfw.MakeContextCurrent(window);

    // Make sure the viewport matches the new window dimensions
    _glfw.SetGlViewport(0, 0, width, height);
}
