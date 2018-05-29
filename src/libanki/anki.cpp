#include "anki.hpp"

#include "zip_archive.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream> // [TODO] temp

using namespace anki;

void anki::import(const path& src) {

    auto archive = zip_archive{src};

    std::cout << std::boolalpha;
    std::cout << "Has .anki21? " << archive.contains("collection.anki21") << '\n';
    std::cout << "Has .anki2?  " << archive.contains("collection.anki2")  << '\n';
    std::cout << std::flush;

    archive.close();
}
