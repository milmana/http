add_library(naive naive.cc ../include/naive.hh)
target_include_directories(naive PUBLIC ../include)
target_compile_features(naive PUBLIC cxx_std_23)
