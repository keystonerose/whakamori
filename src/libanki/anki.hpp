#ifndef LIBANKI_ANKI_HPP
#define LIBANKI_ANKI_HPP

#include "error.hpp"
#include "filesystem.hpp"

namespace anki {

    // [TODO] doc
    // throws zip_error, apkg_error

    void import(const path& src);
}

#endif
