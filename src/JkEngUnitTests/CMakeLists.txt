add_executable(JkEngUnitTests)
if(MSVC)
  target_compile_options(JkEngUnitTests PRIVATE /W4 /WX)
else()
  target_compile_options(JkEngUnitTests PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_sources(JkEngUnitTests
  PRIVATE
    main_test.cpp
    AfterCreatePtrTests.cpp
)
target_include_directories(JkEngUnitTests PRIVATE
  ../
)
target_link_libraries(JkEngUnitTests JkEng ${CONAN_LIBS})
