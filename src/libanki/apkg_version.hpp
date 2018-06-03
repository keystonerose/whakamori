#ifndef LIBANKI_APKG_VERSION_HPP
#define LIBANKI_APKG_VERSION_HPP

#include "filesystem.hpp"

#include <optional>
#include <ostream>

#define LIBANKI_APKG_VERSIONS_X \
    X(anki_2) \
    X(anki_2_1)

namespace anki {

    class zip_archive;

    enum class apkg_version {
        #define X(version) version,
        LIBANKI_APKG_VERSIONS_X
        #undef X
    };

    std::ostream& operator<<(std::ostream& os, apkg_version version);

    // Determines the version number of a specified `apkg` archive. If the archive is not a valid
    // `apkg` or otherwise does not contain a collection file, returns `std::nullopt`.

    auto archive_apkg_version(const zip_archive& archive) -> std::optional<apkg_version>;

    // Returns the relative path within of the main collection file within `apkg` archives of the
    // specified version.

    auto collection_file_path(apkg_version version) -> path;
}

#endif

