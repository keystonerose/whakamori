#ifndef KSR_MAKE_ARRAY_HPP
#define KSR_MAKE_ARRAY_HPP

#include <array>
#include <functional>
#include <type_traits>
#include <utility>

namespace ksr {

    namespace impl::make_array {

        // This largely follows the sample implementation on cppreference for
        // `std::experimental::make_array()`.

        template<typename>
        struct is_ref_wrapper : std::false_type {};

        template<typename t>
        struct is_ref_wrapper<std::reference_wrapper<t>> : std::true_type {};

        template<typename t>
        inline constexpr auto is_ref_wrapper_v = is_ref_wrapper<t>::value;

        template<class t, typename...>
        struct elem_type {
            using type = t;
        };

        template<typename ...arg_ts>
        struct elem_type<void, arg_ts...> {

            static_assert(
                (!is_ref_wrapper_v<arg_ts> && ...),
                "Element type deduction is not supported for std::array objects with one or more "
                "std::reference_wrapper elements");

            using type = std::common_type_t<arg_ts...>;
        };

        template<typename t, typename... arg_ts>
        using elem_type_t = typename elem_type<t, arg_ts...>::type;
    }

    // Shim for `std::array` deduction guide not yet implemented in `libc++`. Constructs an
    // `std::array` object whose size is the number of arguments. If `t` is `void`, further deduces
    // the element type as the common type of `arg_ts...`; otherwise, uses `t` for the element type.
    //
    // Once the `std::array` deduction guide is reliably available from the standard library, that
    // can be used as a drop-in replacement for this function. To match the (apparent?) requirements
    // of the standard, the former case fails to compile if any of the arguments is a
    // `std::reference_wrapper`.

    template<typename t = void, typename ...arg_ts>
    constexpr auto make_array(arg_ts&& ...args)
        -> std::array<impl::make_array::elem_type_t<t, arg_ts...>, sizeof...(arg_ts)> {
        return {std::forward<arg_ts>(args)...};
    }
}

#endif

