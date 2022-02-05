add_library(Window STATIC)
if(MSVC)
  target_compile_options(Window PRIVATE /W4 /WX)
else()
  target_compile_options(Window PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_include_directories(Window INTERFACE include)
target_include_directories(Window PRIVATE include/Window)
target_sources(Window
  PRIVATE
    include/Window/GlfwException.h
    include/Window/IGlfwInputWindow.h
    include/Window/IGlfwWrapper.h
    include/Window/GlfwWrapper.h
    include/Window/IOpenGLWindow.h
    include/Window/GlfwWindow.h
    src/GlfwException.cpp
    src/GlfwWrapper.cpp
    src/GlfwWindow.cpp
)
target_link_libraries(Window ${CONAN_LIBS})
