cmake_minimum_required(VERSION 3.22)

project(
	http
	VERSION 0.1
	DESCRIPTION "multi-threaded HTTP server"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(app)
