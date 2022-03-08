add_library(Window STATIC)
if(MSVC)
  target_compile_options(Window PRIVATE /W4 /WX)
else()
  target_compile_options(Window PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_include_directories(Window INTERFACE include)
target_include_directories(Window PRIVATE include/jkengine/Window)
target_sources(Window
  PRIVATE
    include/jkengine/Window/GlfwException.h
    include/jkengine/Window/IGlfwInputWindow.h
    include/jkengine/Window/IGlfwWrapper.h
    include/jkengine/Window/GlfwWrapper.h
    include/jkengine/Window/IOpenGLWindow.h
    include/jkengine/Window/GlfwWindow.h
    src/GlfwException.cpp
    src/GlfwWrapper.cpp
    src/GlfwWindow.cpp
)
target_link_libraries(Window ${CONAN_LIBS})
