#ifndef KSR_UNDERLYING_TYPE_EXT_HPP
#define KSR_UNDERLYING_TYPE_EXT_HPP

#include <type_traits>

namespace ksr {

    namespace impl {

        template<typename t, bool is_enum>
        struct underlying_type_ext {
            using type = std::remove_cv_t<std::remove_reference_t<t>>;
        };

        template<typename t>
        struct underlying_type_ext<t, true> {
            using type = std::underlying_type_t<t>;
        };
    }

    // Extension of `std::underlying_type` that may be applied to arbitrary types. If `t` is an
    // enumeration type, provides the same behaviour as that standard library facility; otherwise,
    // returns `t` itself, with any cvref-qualification removed (for consistency with the former
    // case).

    template<typename t>
    using underlying_type_ext = impl::underlying_type_ext<t, std::is_enum_v<t>>;

    template<typename t>
    using underlying_type_ext_t = typename underlying_type_ext<t>::type;
}

#endif
