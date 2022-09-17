add_executable(SharedPerformanceTests)
if(MSVC)
  target_compile_options(SharedPerformanceTests PRIVATE /W4 /WX)
else()
  target_compile_options(SharedPerformanceTests PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_sources(SharedPerformanceTests
  PRIVATE
    main_test.cpp
    PoolTests.cpp
)
target_include_directories(SharedPerformanceTests PRIVATE
  ../
)
target_link_libraries(SharedPerformanceTests Shared ${CONAN_LIBS})
