add_executable(JkEng.UnitTests)
if(MSVC)
  target_compile_options(JkEng.UnitTests PRIVATE /W4 /WX)
else()
  target_compile_options(JkEng.UnitTests PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
target_sources(JkEng.UnitTests
  PRIVATE
    main_test.cpp
    AfterCreatePtrTests.cpp
)
target_include_directories(JkEng.UnitTests PRIVATE
  ../
)
target_link_libraries(JkEng.UnitTests JkEng ${CONAN_LIBS})
