add_library(Input STATIC)
if(MSVC)
  target_compile_options(Input PRIVATE /W4 /WX)
else()
  target_compile_options(Input PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_sources(Input
  PRIVATE
    IEngine.h
    IGamepad.h
    Input.h
    Glfw/Engine.h
    Glfw/Engine.cpp
    Glfw/Gamepad.h
    Glfw/Gamepad.cpp
)
target_link_libraries(Input ${CONAN_LIBS})
