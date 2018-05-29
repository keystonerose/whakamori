cmake_minimum_required(VERSION 3.10)
project(libanki)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(LibZip REQUIRED)

add_library(libanki STATIC)
set_property(TARGET libanki PROPERTY CXX_STANDARD 17)

target_sources(libanki PRIVATE
    "anki.cpp"
    "impl/libzip/error.cpp"
    "zip_archive.cpp"
    "zip_file.cpp"
)

target_include_directories(libanki SYSTEM PRIVATE ${LIBZIP_INCLUDE_DIRS})
target_include_directories(libanki PRIVATE ..)

target_link_libraries(libanki ${LIBZIP_LIBRARIES})
