add_executable(JkEng.Input.UnitTests)
if(MSVC)
  target_compile_options(JkEng.Input.UnitTests PRIVATE /W4 /WX)
else()
  target_compile_options(JkEng.Input.UnitTests PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_sources(JkEng.Input.UnitTests
  PRIVATE
    main_test.cpp
    Glfw/EngineTests.cpp
    Glfw/GamepadTests.cpp
    Glfw/MockGlfwInputWrapper.h
    Glfw/FakeGlfwInputWindow.h
)
target_include_directories(JkEng.Input.UnitTests PRIVATE
  ../
)
target_link_libraries(JkEng.Input.UnitTests JkEng.Input ${CONAN_LIBS})
