cmake_minimum_required(VERSION 3.10)
project(ksr_test)

add_executable(ksr_test "")
set_property(TARGET ksr_test PROPERTY CXX_STANDARD 17)

target_sources(ksr_test PRIVATE
    "type_traits/container_traits.cpp"
    "main.cpp"
)

target_include_directories(ksr_test PRIVATE ..)
