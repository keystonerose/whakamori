#ifndef LIBANKI_ERROR_HPP
#define LIBANKI_ERROR_HPP

#include <stdexcept>

#define LIBANKI_ERROR_CODES_ZIP_X \
    X(zip_archive_closed) \
    X(zip_archive_inconsistent) \
    X(zip_archive_read_only) \
    X(zip_bad_alloc) \
    X(zip_bad_crc) \
    X(zip_close_failed) \
    X(zip_file_changed) \
    X(zip_file_delete_failed) \
    X(zip_file_deleted) \
    X(zip_file_exists) \
    X(zip_file_not_found) \
    X(zip_file_open_failed) \
    X(zip_incorrect_password) \
    X(zip_internal_error) \
    X(zip_invalid_archive) \
    X(zip_invalid_argument) \
    X(zip_invalid_compressed_data) \
    X(zip_password_required) \
    X(zip_premature_eof) \
    X(zip_read_error) \
    X(zip_resource_busy) \
    X(zip_seek_error) \
    X(zip_temp_create_failed) \
    X(zip_temp_rename_failed) \
    X(zip_tell_error) \
    X(zip_unsupported_compression_method) \
    X(zip_unsupported_encryption_method) \
    X(zip_unsupported_multi_disk) \
    X(zip_unsupported_operation) \
    X(zip_write_error)

#define LIBANKI_ERROR_CODES_X \
    X(internal_error) \
    X(system_error) \
    X(unsupported_apkg_version) \
    LIBANKI_ERROR_CODES_ZIP_X

namespace anki {

    enum class error_code {
        #define X(err) err,
        LIBANKI_ERROR_CODES_X
        #undef X
    };

    class error : public std::runtime_error {
    public:

        explicit error(error_code code);
        auto code() const noexcept -> error_code { return _code; }

    private:

        error_code _code;
    };
}

#endif
