add_subdirectory(JkEng)
if(BUILD_TESTING)
  add_subdirectory(JkEng.UnitTests)
endif()
add_subdirectory(JkEng.Graphics)
if(BUILD_TESTING)
  add_subdirectory(JkEng.Graphics.UnitTests)
  add_subdirectory(JkEng.Graphics.IntegrationTests)
endif()
add_subdirectory(JkEng.Input)
if(BUILD_TESTING)
  add_subdirectory(JkEng.Input.UnitTests)
endif()
add_subdirectory(JkEng.Physics)
if(BUILD_TESTING)
  add_subdirectory(JkEng.Physics.UnitTests)
  add_subdirectory(JkEng.Physics.PerformanceTests)
endif()
add_subdirectory(JkEng.Window)
