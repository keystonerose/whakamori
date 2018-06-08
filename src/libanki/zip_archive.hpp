#ifndef LIBANKI_ZIP_ARCHIVE_HPP
#define LIBANKI_ZIP_ARCHIVE_HPP

#include "filesystem.hpp"
#include "zip_file.hpp"

#include <utility>

namespace anki {

    // Opaque RAII wrapper for performing a limited number of operations upon a zip archive.
    // Serves to encapsulate use of the underlying library and to enforce consistent error handling.
    // Has two states: open and closed. Some operations may only be performed in the open state.

    class zip_archive {
    public:

        // Opens an archive available from the filesystem path `src`. After construction, the
        // archive is in an open state. Throws `zip_error` on failure.

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

        // Determines whether the archive contains the specified file. The archive must not have
        // been closed. Regardless of the underlying operating system, the path is case-sensitive
        // (and includes directories).  Throws `zip_error` on failure.

        auto contains_file(const path& file_path) const -> bool;

        auto is_open() const -> bool { return _handle != nullptr; }

        // Opens the specified file within the archive and returns an object that can be used to
        // manage it. The archive must not have been closed; if it is closed before the returned
        // file, subsequent operations on the file will throw. Throws `zip_error` on failure,
        // including when the archive does not contain the specified file.

        auto open_file(const path& file_path) const -> zip_file;

        // Closes the archive if it is currently in an open state. May be called to no effect if the
        // archive has already been closed. Throws `zip_error` on failure.

        void close();

    private:

        void* _handle = nullptr;
    };
}

#endif

