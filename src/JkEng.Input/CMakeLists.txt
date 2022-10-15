add_library(JkEng.Input STATIC)
if(MSVC)
  target_compile_options(JkEng.Input PRIVATE /W4 /WX)
else()
  target_compile_options(JkEng.Input PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_include_directories(JkEng.Input INTERFACE include)
target_include_directories(JkEng.Input PRIVATE include/JkEng/Input)
target_sources(JkEng.Input
  PRIVATE
    include/JkEng/Input/IEngine.h
    include/JkEng/Input/IGamepad.h
    include/JkEng/Input/Glfw/Engine.h
    include/JkEng/Input/Glfw/Gamepad.h
    src/Glfw/Engine.cpp
    src/Glfw/Gamepad.cpp
)
target_link_libraries(JkEng.Input JkEng.Window ${CONAN_LIBS})
