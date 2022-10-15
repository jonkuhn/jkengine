add_library(Graphics STATIC)
if(MSVC)
  target_compile_options(Graphics PRIVATE /W4 /WX)
else()
  target_compile_options(Graphics PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_include_directories(Graphics INTERFACE include)
target_include_directories(Graphics PRIVATE include/JkEng/Graphics)
target_sources(Graphics
  PRIVATE
    include/JkEng/Graphics/AfterBuildPtr.h
    include/JkEng/Graphics/Color.h
    include/JkEng/Graphics/Graphics.h
    include/JkEng/Graphics/GridLocation.h
    include/JkEng/Graphics/ICamera2d.h
    include/JkEng/Graphics/IEngine.h
    include/JkEng/Graphics/IImage.h
    include/JkEng/Graphics/IObject2d.h
    include/JkEng/Graphics/PngImage.h
    include/JkEng/Graphics/ILibPngWrapper.h
    include/JkEng/Graphics/LibPngWrapper.h
    include/JkEng/Graphics/IScene.h
    include/JkEng/Graphics/SceneDefinition.h
    include/JkEng/Graphics/IScreenshot.h
    include/JkEng/Graphics/ISprite.h
    include/JkEng/Graphics/SpriteDefinition.h
    include/JkEng/Graphics/TileAtlasDefinition.h
    include/JkEng/Graphics/ITileMap.h
    include/JkEng/Graphics/TileMapDefinition.h
    src/Color.cpp
    src/PngImage.cpp
    include/JkEng/Graphics/OpenGL/Camera2d.h
    include/JkEng/Graphics/OpenGL/Engine.h
    include/JkEng/Graphics/OpenGL/Object2d.h
    include/JkEng/Graphics/OpenGL/OpenGLHelpers.h
    include/JkEng/Graphics/OpenGL/IOpenGLWrapper.h
    include/JkEng/Graphics/OpenGL/OpenGLWrapper.h
    include/JkEng/Graphics/OpenGL/Scene.h
    include/JkEng/Graphics/OpenGL/IShader.h
    include/JkEng/Graphics/OpenGL/Shader.h
    include/JkEng/Graphics/OpenGL/ShaderProgram.h
    include/JkEng/Graphics/OpenGL/Simple3dVertex.h
    include/JkEng/Graphics/OpenGL/SpriteDrawer.h
    include/JkEng/Graphics/OpenGL/SpriteShaderProgram.h
    include/JkEng/Graphics/OpenGL/Texture.h
    include/JkEng/Graphics/OpenGL/TileAtlas.h
    include/JkEng/Graphics/OpenGL/TileMapDrawer.h
    include/JkEng/Graphics/OpenGL/TileMap.h
    include/JkEng/Graphics/OpenGL/TileMapShaderProgram.h
    include/JkEng/Graphics/OpenGL/UnitQuadVertexArray.h
    include/JkEng/Graphics/OpenGL/VertexArray.h
    include/JkEng/Graphics/OpenGL/ViewportCapture.h
    src/OpenGL/Engine.cpp
    src/OpenGL/Object2d.cpp
    src/OpenGL/OpenGLHelpers.cpp
    src/OpenGL/Scene.cpp
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
