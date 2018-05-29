#include "anki.hpp"

#include "zip_archive.hpp"

#include <cassert>
#include <cstdlib>

using namespace anki;

void anki::import(const path& src) {

    auto archive = zip_archive{src};
    archive.close();
}
