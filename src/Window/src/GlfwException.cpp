#include <string>
#include <sstream>

#include "GlfwException.h"

using namespace Window;

namespace
{
    std::string GetGlfwExceptionMessage(IGlfwWrapper& glfw, const std::string& prefix)
    {
        const char* description = nullptr;
        int code = glfw.GetError(&description);
        std::stringstream ss;
        ss << prefix
            << " Code: " << code;
        
        if (description != nullptr)
        {
            ss << " Description: " << std::string(description);
        }

        return ss.str();
    }
}

GlfwException::GlfwException(IGlfwWrapper& glfw, const std::string& prefix)
    : std::runtime_error(GetGlfwExceptionMessage(glfw, prefix))
{

}
