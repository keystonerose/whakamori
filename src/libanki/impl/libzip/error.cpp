#include "error.hpp"

#include "../../zip_archive.hpp"

#include "ksr/final_act.hpp"

namespace anki::impl::libzip {

    auto has_error_excluding(zip_error_t& error, int excluded_code) -> bool {

        if (zip_error_system_type(&error) != ZIP_ET_NONE) {
            return true;
        }

        const auto code = zip_error_code_zip(&error);
        return (code != ZIP_ER_OK)
            && (code != excluded_code);
    }

    void throw_error(zip_error_t& error) {
        throw anki::zip_error{zip_error_strerror(&error)}; // [I18N]
    }

    void throw_error(int code) {

        auto error = zip_error_t{};
        zip_error_init_with_code(&error, code);
        auto const guard = ksr::final_act([&error] { zip_error_fini(&error); });

        throw_error(error);
    }

    void throw_error(zip_t& zip) {

        const auto error = zip_get_error(&zip);
        if (error) {
            throw_error(*error);
        }
        else {
            throw_error(ZIP_ER_INTERNAL);
        }
    }

    [[noreturn]] void throw_error(zip_file_t& file) {
        // [TODO]
    }
}
