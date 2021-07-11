add_library(Graphics STATIC)
if(MSVC)
  target_compile_options(Graphics PRIVATE /W4 /WX)
else()
  target_compile_options(Graphics PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_sources(Graphics
  PRIVATE
    IEngine.h
    IImage.h
    PngImage.h
    PngImage.cpp
    ILibPngWrapper.h
    LibPngWrapper.h
    ITileMap.h
    OpenGL/Engine.h
    OpenGL/Engine.cpp
    OpenGL/GlfwException.h
    OpenGL/GlfwException.cpp
    OpenGL/IGlfwWrapper.h
    OpenGL/GlfwWrapper.h
    OpenGL/GlfwWrapper.cpp
    OpenGL/OpenGLHelpers.h
    OpenGL/OpenGLHelpers.cpp
    OpenGL/IOpenGLWindow.h
    OpenGL/IOpenGLWrapper.h
    OpenGL/OpenGLWrapper.h
    OpenGL/IShader.h
    OpenGL/Shader.h
    OpenGL/Shader.cpp
    OpenGL/ShaderProgram.h
    OpenGL/ShaderProgram.cpp
    OpenGL/Simple3dVertex.h
    OpenGL/Texture.h
    OpenGL/Texture.cpp
    OpenGL/TileMap.h
    OpenGL/TileMap.cpp
    OpenGL/TileMapShaderProgram.h
    OpenGL/TileMapShaderProgram.cpp
    OpenGL/UnitQuadVertexArray.h
    OpenGL/UnitQuadVertexArray.cpp
    OpenGL/VertexArray.h
    OpenGL/Window.h
    OpenGL/Window.cpp
)
target_link_libraries(Graphics ${CONAN_LIBS})
