#ifndef LIBANKI_IMPL_LIBZIP_ERROR_HPP
#define LIBANKI_IMPL_LIBZIP_ERROR_HPP

#include "zip.h"

namespace anki::impl::libzip {

    // Determines whether a libzip error object is in a state representing any error other than the
    // one whose code is `excluded_code`.

    auto has_error_excluding(zip_error_t& error, int excluded_code) -> bool;

    // Throws an `anki::zip_error` exception describing an error state from libzip, reading that
    // error state from a libzip structure if necessary.

    [[noreturn]] void throw_error(zip_error_t& error);
    [[noreturn]] void throw_error(int code);
    [[noreturn]] void throw_error(zip_t& zip);
    [[noreturn]] void throw_error(zip_file_t& file);
}

#endif
