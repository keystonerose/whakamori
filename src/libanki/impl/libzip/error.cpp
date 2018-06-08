#include "error.hpp"

#include "../../error.hpp"
#include "../../zip_archive.hpp"

#include "ksr/algorithm/map_includes.hpp"
#include "ksr/final_act.hpp"

#include <boost/container/flat_map.hpp>

#include <algorithm>
#include <initializer_list>
#include <vector>

using boost::container::flat_map;

namespace anki::impl::libzip {

    namespace {

        using libzip_error_table_t = flat_map<int, error_code>;

        // Determines whether the `libzip_error_table_t` object contains a mapped value for every
        // enumerated `zip_*` error code of `anki::error_code`. This guarantee is provided for the
        // table referenced by `libzip_error_table()`, and calling code need not check; this
        // function is solely for assertion within `libzip_error_table()`.

        auto is_complete(const libzip_error_table_t& table) -> bool {

            static constexpr auto all_versions = {
                #define X(code) error_code::code,
                LIBANKI_ERROR_CODES_ZIP_X
                #undef X
            };

            return ksr::includes_mapped_values(table, all_versions.begin(), all_versions.end());
        }

        // Returns a reference to the lookup table of `anki::error_code` values from libzip error
        // codes, constructing and populating that table on the first call. The referenced table is
        // guaranteed to contain a mapped value for every enumerated `zip_*` error code of
        // `anki::error_code`.

        auto libzip_error_table() -> const libzip_error_table_t& {

            static const auto result = [] {

                auto table = libzip_error_table_t{};
                [[maybe_unused]] auto keys_unique = true;

                keys_unique &= table.try_emplace(ZIP_ER_MULTIDISK,       error_code::zip_unsupported_multi_disk).second;
                keys_unique &= table.try_emplace(ZIP_ER_RENAME,          error_code::zip_temp_rename_failed).second;
                keys_unique &= table.try_emplace(ZIP_ER_CLOSE,           error_code::zip_close_failed).second;
                keys_unique &= table.try_emplace(ZIP_ER_SEEK,            error_code::zip_seek_error).second;
                keys_unique &= table.try_emplace(ZIP_ER_READ,            error_code::zip_read_error).second;
                keys_unique &= table.try_emplace(ZIP_ER_WRITE,           error_code::zip_write_error).second;
                keys_unique &= table.try_emplace(ZIP_ER_CRC,             error_code::zip_bad_crc).second;
                keys_unique &= table.try_emplace(ZIP_ER_ZIPCLOSED,       error_code::zip_archive_closed).second;
                keys_unique &= table.try_emplace(ZIP_ER_NOENT,           error_code::zip_file_not_found).second;
                keys_unique &= table.try_emplace(ZIP_ER_EXISTS,          error_code::zip_file_exists).second;
                keys_unique &= table.try_emplace(ZIP_ER_OPEN,            error_code::zip_file_open_failed).second;
                keys_unique &= table.try_emplace(ZIP_ER_TMPOPEN,         error_code::zip_temp_create_failed).second;
                keys_unique &= table.try_emplace(ZIP_ER_ZLIB,            error_code::zip_internal_error).second;
                keys_unique &= table.try_emplace(ZIP_ER_MEMORY,          error_code::zip_bad_alloc).second;
                keys_unique &= table.try_emplace(ZIP_ER_CHANGED,         error_code::zip_file_changed).second;
                keys_unique &= table.try_emplace(ZIP_ER_COMPNOTSUPP,     error_code::zip_unsupported_compression_method).second;
                keys_unique &= table.try_emplace(ZIP_ER_EOF,             error_code::zip_premature_eof).second;
                keys_unique &= table.try_emplace(ZIP_ER_INVAL,           error_code::zip_invalid_argument).second;
                keys_unique &= table.try_emplace(ZIP_ER_NOZIP,           error_code::zip_invalid_archive).second;
                keys_unique &= table.try_emplace(ZIP_ER_INTERNAL,        error_code::zip_internal_error).second;
                keys_unique &= table.try_emplace(ZIP_ER_INCONS,          error_code::zip_archive_inconsistent).second;
                keys_unique &= table.try_emplace(ZIP_ER_REMOVE,          error_code::zip_file_delete_failed).second;
                keys_unique &= table.try_emplace(ZIP_ER_DELETED,         error_code::zip_file_deleted).second;
                keys_unique &= table.try_emplace(ZIP_ER_ENCRNOTSUPP,     error_code::zip_unsupported_encryption_method).second;
                keys_unique &= table.try_emplace(ZIP_ER_RDONLY,          error_code::zip_archive_read_only).second;
                keys_unique &= table.try_emplace(ZIP_ER_NOPASSWD,        error_code::zip_password_required).second;
                keys_unique &= table.try_emplace(ZIP_ER_WRONGPASSWD,     error_code::zip_incorrect_password).second;
                keys_unique &= table.try_emplace(ZIP_ER_OPNOTSUPP,       error_code::zip_unsupported_operation).second;
                keys_unique &= table.try_emplace(ZIP_ER_INUSE,           error_code::zip_resource_busy).second;
                keys_unique &= table.try_emplace(ZIP_ER_TELL,            error_code::zip_tell_error).second;
                keys_unique &= table.try_emplace(ZIP_ER_COMPRESSED_DATA, error_code::zip_invalid_compressed_data).second;

                assert(keys_unique);
                assert(is_complete(table));

                return table;

            } ();

            return result;
        }

        auto libanki_error_code(int libzip_code) -> error_code {

            static const auto& lookup = libzip_error_table();
            const auto iter = lookup.find(libzip_code);

            return (iter != lookup.end())
                ? iter->second
                : error_code::zip_internal_error;
        }

        auto libanki_error_code(zip_error_t& error) -> error_code {

            switch (zip_error_system_type(&error)) {
            case ZIP_ET_NONE: return libanki_error_code(zip_error_code_zip(&error));
            case ZIP_ET_SYS:  return error_code::system_error;
            case ZIP_ET_ZLIB: return error_code::zip_internal_error;
            }

            assert(false);
            return error_code::internal_error;
        }
    }

    auto has_error_excluding(zip_error_t& error, int excluded_code) -> bool {

        if (zip_error_system_type(&error) != ZIP_ET_NONE) {
            return true;
        }

        const auto code = zip_error_code_zip(&error);
        return (code != ZIP_ER_OK)
            && (code != excluded_code);
    }

    void throw_error(zip_error_t& error) {
        throw anki::error{libanki_error_code(error)};
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

    void throw_error(zip_file_t& file) {

        const auto error = zip_file_get_error(&file);
        if (error) {
            throw_error(*error);
        }
        else {
            throw_error(ZIP_ER_INTERNAL);
        }
    }
}
