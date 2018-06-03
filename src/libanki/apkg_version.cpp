#include "apkg_version.hpp"

#include "filesystem.hpp"
#include "zip_archive.hpp"

#include "ksr/enum.hpp"

#include <boost/container/flat_map.hpp>

#include <algorithm>
#include <type_traits>
#include <vector>

using boost::container::flat_map;

namespace anki {

    namespace {

        // Lookup table record for version-specific properties for `apkg` archives. Provides members
        // for each property available via a function exposed in the header for this module so that
        // those functions may be implemented as a simple lookup in a table constructed once.

        struct apkg_version_info {

            explicit apkg_version_info(const char* collection_file_path)
              : collection_file_path{collection_file_path} {}

            path collection_file_path;
        };

        using apkg_version_table = flat_map<apkg_version, apkg_version_info>;
        using apkg_version_texts = flat_map<apkg_version, const char*>;

        auto version_table() -> const apkg_version_table&;
        auto version_texts() -> const apkg_version_texts&;

        // Determines whether the `apkg_version_table` object contains an element for every
        // enumerated key value of `apkg_version`. This guarantee is provided for the table
        // referenced by `version_table()`, and calling code need not check; this function is solely
        // for assertion within `version_table()`.

        auto is_complete(const apkg_version_table& table) -> bool {

            #define X(version) && (table.find(apkg_version::version) != table.end())
            return true LIBANKI_APKG_VERSIONS_X;
            #undef X
        }

        // Returns a reference to an `apkg_version_info` object describing the version-specific
        // properties for a specified package version. `version` must be an enumerated
        // `apkg_version` value.

        auto version_info(apkg_version version) -> const apkg_version_info& {

            static const auto& lookup = version_table();
            const auto iter = lookup.find(version);
            assert(iter != lookup.end());

            return iter->second;
        }

        // Returns a reference to the lookup table of `apkg_version_info` objects used throughout
        // this module, constructing and populating that table on the first call. The referenced
        // table is guaranteed to contain a mapped `apkg_version_info` value for every enumerated
        // key value of `apkg_version`.

        auto version_table() -> const apkg_version_table& {

            static const auto result = [] {

                auto table = apkg_version_table{};
                [[maybe_unused]] auto keys_unique = true;

                keys_unique &= table.try_emplace(apkg_version::anki_2_1, "collection.anki21").second;
                keys_unique &= table.try_emplace(apkg_version::anki_2,   "collection.anki2").second;

                assert(keys_unique);
                assert(is_complete(table));

                return table;

            } ();

            return result;
        }

        // Returns a text string naming the specified `apkg_version` value, if that value is an
        // enumerated value of `apkg_version`, or `nullptr` otherwise.

        auto version_text(apkg_version version) -> const char* {

            static const auto& lookup = version_texts();
            const auto iter = lookup.find(version);
            return (iter != lookup.end()) ? iter->second : nullptr;
        }

        // Returns a reference to the lookup table of text strings used by `version_text()` to
        // format `apkg_version` values. The referenced table is guaranteed to contain a mapped
        // string for every enumerated key value of `apkg_version`.

        auto version_texts() -> const apkg_version_texts& {

            static const auto result = [] {

                auto texts = apkg_version_texts{};

                #define X(version) texts.try_emplace(apkg_version::version, "apkg_version::" #version);
                LIBANKI_APKG_VERSIONS_X
                #undef X

                return texts;

            } ();

            return result;
        }
    }

    std::ostream& operator<<(std::ostream& os, const apkg_version version) {

        if (const auto text = version_text(version)) {
            os << text;
        }
        else {
            os << "apkg_version{" << ksr::underlying_cast(version) << '}';
        }

        return os;
    }

    auto archive_apkg_version(const zip_archive& archive) -> std::optional<apkg_version> {

        static const auto& lookup = version_table();

        const auto matches_archive = [&archive] (const apkg_version_table::value_type& elem) {
            const auto& info = elem.second;
            return archive.contains_file(info.collection_file_path);
        };

        const auto iter = std::find_if(lookup.begin(), lookup.end(), matches_archive);
        return (iter != lookup.end())
            ? std::optional{iter->first}
            : std::nullopt;
    }

    auto collection_file_path(const apkg_version version) -> path {
        return version_info(version).collection_file_path;
    }
}
