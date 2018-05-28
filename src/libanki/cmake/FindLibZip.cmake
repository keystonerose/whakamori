# Defines the following variables for locating `libzip`:
#
# * `LIBZIP_FOUND`: whether the library is installed on the system;
# * `LIBZIP_INCLUDE_DIRS`: include search paths;
# * `LIBZIP_LIBRARIES`: libraries to link.
# * `LIBZIP_VERSION`: three-component version number for the installed library.

include(FindPackageHandleStandardArgs)
find_package(PkgConfig QUIET)

pkg_check_modules(PC_LIBZIP QUIET libzip)

find_path(LIBZIP_INCLUDE_DIRS
    NAMES zip.h
    HINTS ${PC_LIBZIP_INCLUDE_DIRS}
)

find_library(LIBZIP_LIBRARIES
    NAMES libzip zip
    HINTS ${PC_LIBZIP_LIBRARIES}
)

set(LIBZIP_VERSION ${PC_LIBZIP_VERSION})

find_package_handle_standard_args(LibZip
    FOUND_VAR     LIBZIP_FOUND
    REQUIRED_VARS LIBZIP_INCLUDE_DIRS LIBZIP_LIBRARIES
    VERSION_VAR   LIBZIP_VERSION
)
