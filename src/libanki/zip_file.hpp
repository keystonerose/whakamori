#ifndef LIBANKI_ZIP_FILE_HPP
#define LIBANKI_ZIP_FILE_HPP

#include "filesystem.hpp"

#include <cstddef>
#include <utility>
#include <vector>

namespace anki {

    class zip_archive;

    // RAII wrapper for managing a file opened within a zip archive; see `zip_archive::open_file()`.
    // Has two states: open and closed. When returned from `zip_archive::open_file()`, the object is
    // in an open state. Some operations may only be performed in the open state.

    class zip_file {

        friend zip_archive;

    public:

        // Performs the action of `close()` but does not propagate exceptions. To correctly handle
        // errors arising from close operations, calling code should explicitly call `close()`; the
        // automatic call from the destructor merely ensures attempted clean-up when that calling
        // code exits on an exceptional path.

        ~zip_file();

        zip_file(zip_file&& rhs) noexcept
          : _handle{std::exchange(rhs._handle, nullptr)} {}

        auto operator=(zip_file&& rhs) noexcept -> zip_file& {
            _handle = std::exchange(rhs._handle, nullptr);
            return *this;
        }

        zip_file(const zip_file&) = delete;
        auto operator=(const zip_file&) -> zip_file& = delete;

        auto is_open() const -> bool { return _handle != nullptr; }

        // Reads the contents of the file into memory and returns the loaded byte data. The file
        // must not have been closed. Throws `zip_error` on failure.

        auto read_all() const -> std::vector<std::byte>;

        // Closes the file if it is currently in an open state. May be called to no effect if the
        // file has already been closed. Throws `zip_error` on failure.

        void close();

    private:

        explicit zip_file(void* handle)
          : _handle{handle} {}

        void* _handle = nullptr;
    };
}

#endif
