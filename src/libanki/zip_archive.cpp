#include "zip_archive.hpp"

#include "impl/libzip/error.hpp"
#include "zip_file.hpp"

#include "zip.h"

#include <cassert>

using namespace anki::impl::libzip;

namespace anki {

    namespace {

        auto handle_cast(void* handle) -> zip_t* {
            return static_cast<zip_t*>(handle);
        }
    }

    zip_archive::zip_archive(const path& src) {

        const auto libzip_src = src.string();

        auto error_code = ZIP_ER_OK;
        _handle = zip_open(libzip_src.c_str(), ZIP_RDONLY, &error_code);

        if (!_handle) {
            throw_error(error_code);
        }
    }

    zip_archive::~zip_archive() {

        try {
            close();
        }
        catch (...) {}
    }

    auto zip_archive::contains_file(const path& file_path) const -> bool {

        const auto handle = handle_cast(_handle);
        assert(handle);

        const auto libzip_file = file_path.string();
        const auto result = zip_name_locate(handle, libzip_file.c_str(), 0);

        // Somewhat strangely, `zip_name_locate()` fails and sets a `ZIP_ER_NOENT` when the file is not
        // contained within the archive. So we explicitly exclude that case from error detection.

        const auto error = zip_get_error(handle);
        if (error && has_error_excluding(*error, ZIP_ER_NOENT)) {
            throw_error(*error);
        }

        return result >= 0;
    }

    auto zip_archive::open_file(const path& file_path) const -> zip_file {

        const auto handle = handle_cast(_handle);
        assert(handle);

        const auto libzip_file_path = file_path.string();
        const auto file_handle      = zip_fopen(handle, libzip_file_path.c_str(), 0);

        if (!file_handle) {
            throw_error(*handle);
        }

        return zip_file{file_handle};
    }

    void zip_archive::close() {

        const auto handle = handle_cast(_handle);
        if (!handle) {
            return;
        }

        const bool result = zip_close(handle);
        if (result != 0) {
            throw_error(*handle);
        }

        _handle = nullptr;
    }
}
