add_executable(InputUnitTests)
if(MSVC)
  target_compile_options(InputUnitTests PRIVATE /W4 /WX)
else()
  target_compile_options(InputUnitTests PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_sources(InputUnitTests
  PRIVATE
    main_test.cpp
    Glfw/EngineTests.cpp
    Glfw/GamepadTests.cpp
    Glfw/MockGlfwInputWrapper.h
    Glfw/FakeGlfwInputWindow.h
)
target_include_directories(InputUnitTests PRIVATE
  ../
)
target_link_libraries(InputUnitTests Input ${CONAN_LIBS})
