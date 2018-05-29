#include "zip_archive.hpp"

#include "ksr/final_act.hpp"

#include "zip.h"

#include <cassert>
#include <string>

using namespace anki;

namespace {

    auto handle_cast(void* handle) -> zip_t* {
        return static_cast<zip_t*>(handle);
    }

    auto make_zip_error(int libzip_error_code) -> anki::zip_error {

        auto error = zip_error_t{};
        zip_error_init_with_code(&error, libzip_error_code);
        auto const guard = ksr::final_act([&error] { zip_error_fini(&error); });

        return anki::zip_error{zip_error_strerror(&error)};
    }
}

zip_archive::zip_archive(const path& src) {

    auto error_code = ZIP_ER_OK;
    const auto src_str = src.string();
    _handle = zip_open(src_str.c_str(), ZIP_RDONLY, &error_code);

    if (!_handle) {
        throw make_zip_error(error_code);
    }
}

zip_archive::~zip_archive() {

    try {
        close();
    }
    catch (...) {}
}

void zip_archive::close() {

    if (!is_open()) {
        return;
    }

    // If errors occur when closing the archive, there's not much we can know about the nature
    // of those errors without re-opening it.

    const auto handle = handle_cast(_handle);
    const auto result = zip_close(handle);

    if (result != -1) {
        throw make_zip_error(ZIP_ER_CLOSE);
    }
}
