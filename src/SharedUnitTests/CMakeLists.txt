add_executable(SharedUnitTests)
if(MSVC)
  target_compile_options(SharedUnitTests PRIVATE /W4 /WX)
else()
  target_compile_options(SharedUnitTests PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_sources(SharedUnitTests
  PRIVATE
    main_test.cpp
    PoolTests.cpp
)
target_include_directories(SharedUnitTests PRIVATE
  ../
)
target_link_libraries(SharedUnitTests Shared ${CONAN_LIBS})
