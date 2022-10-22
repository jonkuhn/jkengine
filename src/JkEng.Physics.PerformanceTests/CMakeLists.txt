add_executable(JkEng.Physics.PerformanceTests)
if(MSVC)
  target_compile_options(JkEng.Physics.PerformanceTests PRIVATE /W4 /WX)
else()
  target_compile_options(JkEng.Physics.PerformanceTests PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_sources(JkEng.Physics.PerformanceTests
  PRIVATE
    main_test.cpp
    MovableAabb2dTests.cpp
)
target_include_directories(JkEng.Physics.PerformanceTests
  PRIVATE
    $<TARGET_PROPERTY:JkEng.Physics,INCLUDE_DIRECTORIES>
)
target_link_libraries(JkEng.Physics.PerformanceTests JkEng.Physics ${CONAN_LIBS})
