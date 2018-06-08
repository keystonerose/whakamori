#include "anki.hpp"

#include "apkg_version.hpp"
#include "zip_archive.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream> // [TODO] temp

namespace anki {

    void import(const path& src) {

        auto archive = zip_archive{src};
        const auto version = archive_apkg_version(archive);
        if (!version) {
            throw error{error_code::unsupported_apkg_version};
        }

        const auto collection_path = collection_file_path(*version);

        auto collection_file  = archive.open_file(collection_path);
        auto collection_bytes = collection_file.read_all();

        collection_file.close();
        archive.close();
    }
}
