add_executable(JkEng.Graphics.IntegrationTests)
if(MSVC)
  target_compile_options(JkEng.Graphics.IntegrationTests PRIVATE /W4 /WX)
else()
  target_compile_options(JkEng.Graphics.IntegrationTests PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_sources(JkEng.Graphics.IntegrationTests
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
    Window/WindowTests.cpp
)
target_link_libraries(JkEng.Graphics.IntegrationTests JkEng.Graphics ${CONAN_LIBS})

file(COPY ./TestFiles/test31x47.png DESTINATION ${CMAKE_BINARY_DIR}/bin/TestFiles)
file(COPY ./TestFiles/test31x47.jpg DESTINATION ${CMAKE_BINARY_DIR}/bin/TestFiles)
file(COPY ./TestFiles/colortiles4x4.png DESTINATION ${CMAKE_BINARY_DIR}/bin/TestFiles)
file(COPY ./TestFiles/colortiles4x4emptycenters.png DESTINATION ${CMAKE_BINARY_DIR}/bin/TestFiles)
file(COPY ./TestFiles/BlackAndWhiteCheckerBoard2TilesBy2Tiles16x16.png DESTINATION ${CMAKE_BINARY_DIR}/bin/TestFiles)
file(COPY ./TestFiles/BlackAndWhiteCheckerBoard2TilesBy2Tiles64x64.png DESTINATION ${CMAKE_BINARY_DIR}/bin/TestFiles)
file(COPY ./TestFiles/16x16WithBorderTestPatterns.png DESTINATION ${CMAKE_BINARY_DIR}/bin/TestFiles)
file(COPY ./TestFiles/SingleTileBlackRedWhiteBlue.png DESTINATION ${CMAKE_BINARY_DIR}/bin/TestFiles)
