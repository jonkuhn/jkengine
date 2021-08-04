add_executable(GraphicsIntegrationTests)
if(MSVC)
  target_compile_options(GraphicsIntegrationTests PRIVATE /W4 /WX)
else()
  target_compile_options(GraphicsIntegrationTests PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_sources(GraphicsIntegrationTests
  PRIVATE
    main_test.cpp
    ColorTiles4x4.h
    PngImageTests.cpp
    ScreenSampleHelpers.h
    SpriteTests.cpp
    TileMapTests.cpp
    OpenGL/TestHelpers.h
    OpenGL/TestHelpers.cpp
    OpenGL/TextureTests.cpp
    OpenGL/ShaderTests.cpp
    OpenGL/ShaderProgramTests.cpp
    OpenGL/VertexArrayTests.cpp
    OpenGL/WindowTests.cpp
)
target_include_directories(GraphicsIntegrationTests PRIVATE
  ../
)
target_link_libraries(GraphicsIntegrationTests Graphics ${CONAN_LIBS})

file(COPY ./TestFiles/test31x47.png DESTINATION ${CMAKE_BINARY_DIR}/bin/TestFiles)
file(COPY ./TestFiles/test31x47.jpg DESTINATION ${CMAKE_BINARY_DIR}/bin/TestFiles)
file(COPY ./TestFiles/colortiles4x4.png DESTINATION ${CMAKE_BINARY_DIR}/bin/TestFiles)
file(COPY ./TestFiles/colortiles4x4emptycenters.png DESTINATION ${CMAKE_BINARY_DIR}/bin/TestFiles)
