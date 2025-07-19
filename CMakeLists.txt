cmake_minimum_required(VERSION 3.22...4.0)

project(
	naive-http
	VERSION 0.1
	DESCRIPTION "Naive implementation of multi-threaded HTTP server"
	LANGUAGES CXX)

add_subdirectory(apps)
add_subdirectory(src)
