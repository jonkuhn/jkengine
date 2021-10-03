add_library(JkEng.Physics STATIC)
if(MSVC)
  target_compile_options(JkEng.Physics PRIVATE /W4 /WX)
else()
  target_compile_options(JkEng.Physics PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_include_directories(JkEng.Physics INTERFACE include)
target_include_directories(JkEng.Physics PRIVATE include/JkEng/Physics)
target_include_directories(JkEng.Physics PRIVATE src)
target_sources(JkEng.Physics
  PRIVATE
    include/JkEng/Physics/Engine.h
    include/JkEng/Physics/IMovableAabb2d.h
    include/JkEng/Physics/IReadOnlyAabb2d.h
    include/JkEng/Physics/IScene.h
    include/JkEng/Physics/MovableAabb2dDefinition.h
    include/JkEng/Physics/SceneDefinition.h
    src/Aabb.h
    src/Aabb.cpp
    src/Engine.cpp
    src/Scene.h
    src/Scene.cpp
    src/SnapshotOfReadOnlyAabb2d.h
)
target_link_libraries(JkEng.Physics JkEng ${CONAN_LIBS})
