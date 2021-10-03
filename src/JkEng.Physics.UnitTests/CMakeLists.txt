add_executable(JkEng.Physics.UnitTests)
if(MSVC)
  target_compile_options(JkEng.Physics.UnitTests PRIVATE /W4 /WX)
else()
  target_compile_options(JkEng.Physics.UnitTests PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_sources(JkEng.Physics.UnitTests
  PRIVATE
    main_test.cpp
    AabbTests.cpp
    EngineTests.cpp
)
target_include_directories(JkEng.Physics.UnitTests
  PRIVATE
    $<TARGET_PROPERTY:JkEng.Physics,INCLUDE_DIRECTORIES>
)
target_link_libraries(JkEng.Physics.UnitTests JkEng.Physics ${CONAN_LIBS})
