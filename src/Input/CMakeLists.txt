add_library(Input STATIC)
if(MSVC)
  target_compile_options(Input PRIVATE /W4 /WX)
else()
  target_compile_options(Input PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_include_directories(Input INTERFACE include)
target_include_directories(Input PRIVATE include/jkengine/Input)
target_sources(Input
  PRIVATE
    include/jkengine/Input/IEngine.h
    include/jkengine/Input/IGamepad.h
    include/jkengine/Input/Glfw/Engine.h
    include/jkengine/Input/Glfw/Gamepad.h
    src/Glfw/Engine.cpp
    src/Glfw/Gamepad.cpp
)
target_link_libraries(Input Window ${CONAN_LIBS})
