add_library(Graphics STATIC)
if(MSVC)
  target_compile_options(Graphics PRIVATE /W4 /WX)
else()
  target_compile_options(Graphics PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_include_directories(Graphics INTERFACE include)
target_include_directories(Graphics PRIVATE include/jkengine/Graphics)
target_sources(Graphics
  PRIVATE
    include/jkengine/Graphics/Color.h
    include/jkengine/Graphics/Graphics.h
    include/jkengine/Graphics/GridLocation.h
    include/jkengine/Graphics/ICamera2d.h
    include/jkengine/Graphics/IEngine.h
    include/jkengine/Graphics/IImage.h
    include/jkengine/Graphics/IObject2d.h
    include/jkengine/Graphics/PngImage.h
    include/jkengine/Graphics/ILibPngWrapper.h
    include/jkengine/Graphics/LibPngWrapper.h
    include/jkengine/Graphics/IScreenshot.h
    include/jkengine/Graphics/ISprite.h
    include/jkengine/Graphics/ITileAtlas.h
    include/jkengine/Graphics/ITileMap.h
    src/Color.cpp
    src/PngImage.cpp
    include/jkengine/Graphics/OpenGL/Camera2d.h
    include/jkengine/Graphics/OpenGL/Engine.h
    include/jkengine/Graphics/OpenGL/Object2d.h
    include/jkengine/Graphics/OpenGL/OpenGLHelpers.h
    include/jkengine/Graphics/OpenGL/IOpenGLWrapper.h
    include/jkengine/Graphics/OpenGL/OpenGLWrapper.h
    include/jkengine/Graphics/OpenGL/IShader.h
    include/jkengine/Graphics/OpenGL/Shader.h
    include/jkengine/Graphics/OpenGL/ShaderProgram.h
    include/jkengine/Graphics/OpenGL/Simple3dVertex.h
    include/jkengine/Graphics/OpenGL/SpriteDrawer.h
    include/jkengine/Graphics/OpenGL/SpriteShaderProgram.h
    include/jkengine/Graphics/OpenGL/Texture.h
    include/jkengine/Graphics/OpenGL/TileAtlas.h
    include/jkengine/Graphics/OpenGL/TileMapDrawer.h
    include/jkengine/Graphics/OpenGL/TileMap.h
    include/jkengine/Graphics/OpenGL/TileMapShaderProgram.h
    include/jkengine/Graphics/OpenGL/UnitQuadVertexArray.h
    include/jkengine/Graphics/OpenGL/VertexArray.h
    include/jkengine/Graphics/OpenGL/ViewportCapture.h
    src/OpenGL/Engine.cpp
    src/OpenGL/Object2d.cpp
    src/OpenGL/OpenGLHelpers.cpp
    src/OpenGL/Shader.cpp
    src/OpenGL/ShaderProgram.cpp
    src/OpenGL/SpriteShaderProgram.cpp
    src/OpenGL/Texture.cpp
    src/OpenGL/TileAtlas.cpp
    src/OpenGL/TileMapShaderProgram.cpp
    src/OpenGL/UnitQuadVertexArray.cpp
    src/OpenGL/ViewportCapture.cpp
)
target_link_libraries(Graphics Window Shared ${CONAN_LIBS})
