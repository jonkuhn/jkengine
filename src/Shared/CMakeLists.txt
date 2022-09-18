add_library(Shared INTERFACE)
target_include_directories(Shared INTERFACE include)
target_sources(Shared
  INTERFACE
    include/jkengine/Shared/Pool.h
    include/jkengine/Shared/Pool2.h
    include/jkengine/Shared/Pool3.h
)
