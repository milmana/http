add_library(lib lib.cc ../include/lib.hh)
target_include_directories(lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
