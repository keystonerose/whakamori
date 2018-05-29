#ifndef LIBANKI_ZIP_ARCHIVE_HPP
#define LIBANKI_ZIP_ARCHIVE_HPP

#include "filesystem.hpp"

#include <stdexcept>
#include <utility>

namespace anki {

    // Opaque RAII wrapper for performing a limited number of operations upon a zip archive.
    // Serves to encapsulate use of the underlying library and to enforce consistent error handling.
    // Has two states: open and closed. Most operations may only be performed

    class zip_archive {
    public:

        // Opens an archive available from the filesystem path `src`. After construction, the
        // archive is in an open state. Throws `zip_error` if the archive cannot be opened.

        explicit zip_archive(const path& src);

        // Performs the action of `close()` but does not propagate exceptions. To correctly handle
        // errors arising from close operations, calling code should explicitly call `close()`; the
        // automatic call from the destructor merely ensures attempted clean-up when that calling
        // code exits on an exceptional path.

        ~zip_archive();

        zip_archive(zip_archive&& rhs) noexcept
          : _handle{std::exchange(rhs._handle, nullptr)} {}

        auto operator=(zip_archive&& rhs) noexcept -> zip_archive& {
            _handle = std::exchange(rhs._handle, nullptr);
            return *this;
        }

        zip_archive(const zip_archive&) = delete;
        auto operator=(const zip_archive&) -> zip_archive& = delete;

        auto is_open() const -> bool { return _handle != nullptr; }

        // Closes the archive if it has not been closed already. Throws `zip_error` if the archive
        // cannot be closed cleanly.

        void close();

    private:

        void* _handle = nullptr;
    };

    class zip_error : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };
}

#endif

