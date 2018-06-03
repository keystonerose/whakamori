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
        if (version) {
            std::cout << "Archive version: " << *version << '\n';
        }
        else {
            std::cout << "Unknown archive version.\n";
        }

        std::cout << std::flush;
        archive.close();
    }
}
