#ifndef KSR_NARROW_CAST_HPP
#define KSR_NARROW_CAST_HPP

#include "type_traits/is_numeric.hpp"

#include <cassert>

namespace ksr {

    namespace impl::narrow_cast {

        // Determines whether a specified arithmetic or enumeration value is negative; that is, less
        // than a zero-initialized value of the same type.

        template<typename t>
        constexpr auto is_negative(t value) -> bool {

            static_assert(is_numeric_v<t>);

            using underlying_type = underlying_type_ext_t<t>;
            return static_cast<underlying_type>(value) < underlying_type{};
        }
    }

    // Using `static_cast`, performs a narrowing conversion of the specified value to a
    // corresponding value of type `dst_type`. For release builds, the behaviour of this function is
    // identical to that of `static_cast` and it generates no machine code; for debug builds, a
    // check is made to ensure that the input value is within the range of `dst_type`.
    //
    // This is similar in spirit to `gsl::narrow()`, but extends that function to also cover
    // enumeration types and, in providing identical behaviour to `static_cast` for release builds,
    // facilitates more extensive use.
    //
    // `src_type` and `dst_type` must each be either arithmetic or enumeration types (that is,
    // "numeric" in the sense of `ksr::is_numeric`).

    template<typename dst_type, typename src_type>
    constexpr auto narrow_cast(src_type value) -> dst_type {

        using namespace impl::narrow_cast;

        static_assert(is_numeric_v<src_type>);
        static_assert(is_numeric_v<dst_type>);

        const auto result = static_cast<dst_type>(value);

        assert(static_cast<src_type>(result) == value);
        assert(is_negative(result) == is_negative(value));

        return result;
    }
}

#endif
