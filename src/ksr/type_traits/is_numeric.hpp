#ifndef KSR_IS_NUMERIC_HPP
#define KSR_IS_NUMERIC_HPP

#include "underlying_type_ext.hpp"

#include <type_traits>

namespace ksr {

    // Determines whether the type `t` is arithmetic once transformed to any potentially underlying
    // type as per `ksr::underlying_type_ext`. Crucially, this includes enumeration types as well as
    // those that would normally be identified by `std::is_arithmetic`.

    template<typename t>
    using is_numeric = std::is_arithmetic<underlying_type_ext_t<t>>;

    template<typename t>
    inline constexpr auto is_numeric_v = is_numeric<t>::value;
}

#endif
