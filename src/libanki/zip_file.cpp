#include "zip_file.hpp"

#include "impl/libzip/error.hpp"

#include "ksr/narrow_cast.hpp"

#include "zip.h"

#include <cassert>
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

        static constexpr auto chunk_size = std::size_t{8192}; // Arbitrary; not profiled

        const auto handle = handle_cast(_handle);
        assert(handle);

        auto bytes     = std::vector<std::byte>{};
        auto pos       = std::size_t{0};
        auto size_read = std::size_t{0};

        do {

            // Each chunk of `bytes` is unnecessarily zeroed before being populated with data read
            // from the file; this seems unlikely to be a performance problem though.

            pos = bytes.size();
            bytes.resize(pos + chunk_size);

            const auto wide_size_read = std::int64_t{zip_fread(handle, &bytes[pos], chunk_size)};
            if (wide_size_read >= 0) {
                size_read = ksr::narrow_cast<std::size_t>(wide_size_read);
            }
            else {
                throw_error(*handle);
            }
        }
        while (size_read == chunk_size);

        assert(size_read < chunk_size);
        const auto unused_size = chunk_size - size_read;

        assert(unused_size < bytes.size());
        bytes.resize(bytes.size() - unused_size);

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
