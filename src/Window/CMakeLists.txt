add_library(Window STATIC)
if(MSVC)
  target_compile_options(Window PRIVATE /W4 /WX)
else()
  target_compile_options(Window PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_sources(Window
  PRIVATE
    GlfwException.h
    GlfwException.cpp
    IGlfwWrapper.h
    GlfwWrapper.h
    GlfwWrapper.cpp
    IOpenGLWindow.h
    GlfwWindow.h
    GlfwWindow.cpp
)
target_link_libraries(Window ${CONAN_LIBS})
