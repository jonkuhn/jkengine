add_library(JkEng.Window STATIC)
if(MSVC)
  target_compile_options(JkEng.Window PRIVATE /W4 /WX)
else()
  target_compile_options(JkEng.Window PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_include_directories(JkEng.Window INTERFACE include)
target_include_directories(JkEng.Window PRIVATE include/JkEng/Window)
target_sources(JkEng.Window
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
target_link_libraries(JkEng.Window ${CONAN_LIBS})
