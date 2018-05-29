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

    auto has_error_excluding(zip_error_t& error, int excluded_code) -> bool {

        if (zip_error_system_type(&error) != ZIP_ET_NONE) {
            return true;
        }

        const auto code = zip_error_code_zip(&error);
        return (code != ZIP_ER_OK)
            && (code != excluded_code);
    }

    auto libzip_path(const path& generic_path) -> std::string {
        return generic_path.string();
    }

    [[noreturn]] void throw_libzip_error(zip_error_t& error) {
        throw anki::zip_error{zip_error_strerror(&error)}; // [I18N]
    }

    [[noreturn]] void throw_libzip_error(int code) {

        auto error = zip_error_t{};
        zip_error_init_with_code(&error, code);
        auto const guard = ksr::final_act([&error] { zip_error_fini(&error); });

        throw_libzip_error(error);
    }

    [[noreturn]] void throw_libzip_error(zip_t& zip) {

        const auto error = zip_get_error(&zip);
        if (error) {
            throw_libzip_error(*error);
        }
        else {
            throw_libzip_error(ZIP_ER_INTERNAL);
        }
    }
}

zip_archive::zip_archive(const path& src) {

    const auto libzip_src = libzip_path(src);

    auto error_code = ZIP_ER_OK;
    _handle = zip_open(libzip_src.c_str(), ZIP_RDONLY, &error_code);

    if (!_handle) {
        throw_libzip_error(error_code);
    }
}

zip_archive::~zip_archive() {

    try {
        close();
    }
    catch (...) {}
}

auto zip_archive::contains(const path& file) const -> bool {

    const auto handle = handle_cast(_handle);
    if (!handle) {
        return false;
    }

    const auto libzip_file = libzip_path(file);
    const auto result = zip_name_locate(handle, libzip_file.c_str(), 0);

    // Somewhat strangely, `zip_name_locate()` fails and sets a `ZIP_ER_NOENT` when the file is not
    // contained within the archive. So we explicitly exclude that case from error detection.

    const auto error = zip_get_error(handle);
    if (error && has_error_excluding(*error, ZIP_ER_NOENT)) {
        throw_libzip_error(*error);
    }

    return result >= 0;
}

void zip_archive::close() {

    const auto handle = handle_cast(_handle);
    if (!handle) {
        return;
    }

    const bool result = zip_close(handle);
    if (result != 0) {
        throw_libzip_error(*handle);
    }

    _handle = nullptr;
}
