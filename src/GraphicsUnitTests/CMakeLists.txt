add_executable(GraphicsUnitTests)
if(MSVC)
  target_compile_options(GraphicsUnitTests PRIVATE /W4 /WX)
else()
  target_compile_options(GraphicsUnitTests PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_sources(GraphicsUnitTests
  PRIVATE
    main_test.cpp
    FakeImage.h
    PngImageTests.cpp
    TestHelpers.h
    OpenGL/MockGlfwWrapper.h
    OpenGL/MockOpenGLWrapper.h
    OpenGL/MockShader.h
    OpenGL/Object2dTests.cpp
    OpenGL/ShaderProgramTests.cpp
    OpenGL/ShaderTests.cpp
    OpenGL/TextureTests.cpp
    OpenGL/VertexArrayTests.cpp
    OpenGL/WindowTests.cpp
    MockLibPngWrapper.h
)
target_include_directories(GraphicsUnitTests PRIVATE
  ../
)
target_link_libraries(GraphicsUnitTests Graphics ${CONAN_LIBS})
