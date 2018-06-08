#include "error.hpp"

#include "ksr/enum.hpp"

#include <boost/container/flat_map.hpp>

#include <sstream>
#include <string>

using boost::container::flat_map;

namespace anki {

    namespace {

        using error_code_texts_t = flat_map<error_code, const char*>;

        auto error_code_texts() -> const error_code_texts_t&;

        // Returns a text string naming the specified `error_code` value, if that value is an
        // enumerated value of `error_code`, or "error_code{X}" otherwise (where X is the underlying
        // integral value of `code`).

        auto error_code_text(error_code code) -> std::string {

            static const auto& lookup = error_code_texts();
            const auto iter = lookup.find(code);

            if (iter != lookup.end()) {
                return std::string{iter->second};
            }
            else {

                std::ostringstream oss;
                oss << "error_code{" << ksr::underlying_cast(code) << '}';
                return oss.str();
            }
        }

        // Returns a reference to the lookup table of text strings used by `error_code_text()` to
        // format `error_code` values. The referenced table is guaranteed to contain a mapped
        // string for every enumerated key value of `error_code`.

        auto error_code_texts() -> const error_code_texts_t& {

            static const auto result = [] {

                auto texts = error_code_texts_t{};

                #define X(code) texts.try_emplace(error_code::code, #code);
                LIBANKI_ERROR_CODES_X
                #undef X

                return texts;

            } ();

            return result;
        }
    }

    error::error(const error_code code)
      : runtime_error{error_code_text(code)}, _code{code} {
    }
}
