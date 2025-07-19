add_executable(server server.cc)
target_compile_features(server PRIVATE cxx_std_23)
target_link_libraries(server PRIVATE naive)
