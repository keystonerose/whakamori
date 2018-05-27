cmake_minimum_required(VERSION 3.10)
project(libanki)

find_package(ZLIB REQUIRED)

add_library(libanki STATIC)
set_property(TARGET libanki PROPERTY CXX_STANDARD 17)

target_sources(libanki PRIVATE
    "import.cpp"
)

target_include_directories(libanki SYSTEM PRIVATE ${ZLIB_INCLUDE_DIRS})
target_link_libraries(libanki ${ZLIB_LIBRARIES})
