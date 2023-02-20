add_executable(JkEng.Graphics.UnitTests)
if(MSVC)
  target_compile_options(JkEng.Graphics.UnitTests PRIVATE /W4 /WX)
else()
  target_compile_options(JkEng.Graphics.UnitTests PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_sources(JkEng.Graphics.UnitTests
  PRIVATE
    main_test.cpp
    FakeImage.h
    PngImageTests.cpp
    TestHelpers.h
    TileAtlasDefinitionTests.cpp
    OpenGL/MockOpenGLWrapper.h
    OpenGL/MockShader.h
    OpenGL/Object2dTests.cpp
    OpenGL/ShaderProgramTests.cpp
    OpenGL/ShaderTests.cpp
    OpenGL/TextureTests.cpp
    OpenGL/VertexArrayTests.cpp
    Window/MockGlfwWrapper.h
    Window/WindowTests.cpp
    MockLibPngWrapper.h
)
target_include_directories(JkEng.Graphics.UnitTests
  PRIVATE
    $<TARGET_PROPERTY:JkEng.Graphics,INCLUDE_DIRECTORIES>
)
target_link_libraries(JkEng.Graphics.UnitTests JkEng.Graphics ${CONAN_LIBS})
