cmake_minimum_required(VERSION 3.10)
project(whakamori)

set(SRC_DIR "src")

add_subdirectory("${SRC_DIR}/ksr_test")
add_subdirectory("${SRC_DIR}/libanki")

add_executable(whakamori "")

add_dependencies(whakamori libanki)

set_property(TARGET whakamori PROPERTY CXX_STANDARD 17)

target_include_directories(whakamori PRIVATE ${SRC_DIR})

include("${SRC_DIR}/CMakeLists.txt")

target_link_libraries(whakamori libanki stdc++fs)
install(TARGETS whakamori RUNTIME DESTINATION bin)
