add_library(JkEng INTERFACE)
target_include_directories(JkEng INTERFACE include)
target_sources(JkEng
  INTERFACE
  include/JkEng/AfterCreatePtr.h
)
