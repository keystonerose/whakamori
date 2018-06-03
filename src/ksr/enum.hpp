#ifndef KSR_ENUM_HPP
#define KSR_ENUM_HPP

#include <type_traits>

namespace ksr {

    template<typename enum_t>
    auto underlying_cast(enum_t value) {

        static_assert(std::is_enum_v<enum_t>);
        using underlying_type = std::underlying_type_t<enum_t>;
        return static_cast<underlying_type>(value);
    }
}

#endif
