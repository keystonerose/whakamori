#include "libanki/anki.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

auto main() -> int {

    try {

        anki::import("decks.apkg");
    }
    catch (const std::exception& ex) {

        std::cout << ex.what() << '\n';
        std::cout << std::flush;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
