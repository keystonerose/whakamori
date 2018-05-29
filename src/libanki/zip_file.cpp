#include "zip_file.hpp"

#include "impl/libzip/error.hpp"

#include "ksr/narrow_cast.hpp"

#include "zip.h"

#include <cstdint>
#include <cstddef>

using namespace anki::impl::libzip;

namespace anki {

    namespace {

        auto handle_cast(void* handle) -> zip_file_t* {
            return static_cast<zip_file_t*>(handle);
        }
    }

    zip_file::~zip_file() {

        try {
            close();
        }
        catch (...) {}
    }

    auto zip_file::read_all() const -> std::vector<std::byte> {

        const auto handle = handle_cast(_handle);
        assert(handle);

        if (zip_fseek(handle, 0, SEEK_END) != 0) {
            throw_error(*handle);
        }

        const auto wide_size = std::int64_t{zip_ftell(handle)};
        if (wide_size < 0) {
            throw_error(*handle);
        }

        if (zip_fseek(handle, 0, SEEK_SET) != 0) {
            throw_error(*handle);
        }

        const auto size = ksr::narrow_cast<std::size_t>(wide_size);
        auto bytes = std::vector<std::byte>{};
        bytes.resize(size);

        if (zip_fread(handle, bytes.data(), size) != 0) {
            throw_error(*handle);
        }

        return bytes;
    }

    void zip_file::close() {

        const auto handle = handle_cast(_handle);
        if (!handle) {
            return;
        }

        const auto error_code = zip_fclose(handle);
        if (error_code != 0) {
            throw_error(error_code);
        }

        _handle = nullptr;
    }
}
