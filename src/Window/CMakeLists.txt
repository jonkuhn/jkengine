add_library(Window STATIC)
if(MSVC)
  target_compile_options(Window PRIVATE /W4 /WX)
else()
  target_compile_options(Window PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_include_directories(Window INTERFACE include)
target_include_directories(Window PRIVATE include/JkEng/Window)
target_sources(Window
  PRIVATE
    include/JkEng/Window/GlfwException.h
    include/JkEng/Window/IGlfwInputWindow.h
    include/JkEng/Window/IGlfwWrapper.h
    include/JkEng/Window/GlfwWrapper.h
    include/JkEng/Window/IOpenGLWindow.h
    include/JkEng/Window/GlfwWindow.h
    src/GlfwException.cpp
    src/GlfwWrapper.cpp
    src/GlfwWindow.cpp
)
target_link_libraries(Window ${CONAN_LIBS})
