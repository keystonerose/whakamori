#ifndef KSR_TYPE_TRAITS_CONTAINER_TRAITS_HPP
#define KSR_TYPE_TRAITS_CONTAINER_TRAITS_HPP

#include <iterator>
#include <type_traits>
#include <utility>

namespace ksr {

    namespace impl::iterator_type {

        using std::begin;
        using std::cbegin;

        template<typename t, typename>
        struct result {
            using type = std::decay_t<decltype(begin(std::declval<t>()))>;
        };

        template<typename t>
        struct result<t, std::void_t<typename t::iterator>> {
            using type = typename t::iterator;
        };

        template<typename t>
        struct result<const t, std::void_t<typename t::const_iterator>> {
            using type = typename t::const_iterator;
        };
    }

    // Given a container type `t`, determines the iterator type for that container. Provides the
    // member type `type` which is an alias for either:
    // * `t::iterator` if that nested type exists and `t` is non-`const`;
    // * `t::const_iterator` if that nested type exists and `t` is `const`; or
    // * the type returned by `begin(t)` otherwise.
    //
    // If no appropriate `t::iterator_type` or `t::const_iterator` alias exists, then `t` must be a
    // type such that `begin(inst)` is well-formed (where `inst` is an instance of `t` and a
    // `using std::begin` directive is visible).

    template<typename t>
    struct iterator_type {
        using type = typename impl::iterator_type::result<t, void>::type;
    };

    template<typename t>
    using iterator_type_t = typename iterator_type<t>::type;

    namespace impl::value_type {

        template<typename t, typename>
        struct result {
            using type = typename std::iterator_traits<iterator_type_t<t>>::value_type;
        };

        template<typename t>
        struct result<t, std::void_t<typename t::value_type>> {
            using type = typename t::value_type;
        };
    }

    // Given a container type `t`, determines the value type for that container. Provides the member
    // type `type` which is an alias for either:
    // * `t::value_type` if that nested type exists; or
    // * the value type of `iterator_type_t<t>` otherwise.
    //
    // If no `t::value_type` alias exists, then `t` must be a type such that `begin(inst)` is well-
    // -formed and returns an iterator compatible with `std::iterator_traits` (where `inst` is an
    // instance of `t` and a `using std::begin` directive is visible).

    template<typename t>
    struct value_type {
        using type = typename impl::value_type::result<t, void>::type;
    };

    template<typename t>
    using value_type_t = typename value_type<t>::type;

    namespace impl::mapped_type {

        template<typename t, typename>
        struct result {
            using type = std::tuple_element_t<1, value_type_t<t>>;
        };

        template<typename t>
        struct result<t, std::void_t<typename t::mapped_type>> {
            using type = typename t::mapped_type;
        };
    }

    // Given a map-like container type `t`, determines the mapped type for that container. Provides
    // the member type `type` which is an alias for either:
    // * `t::mapped_type` if that nested type exists; or
    // * the second tuple-element type of `value_type_t<t>` otherwise.
    //
    // If no `t::mapped_type` alias exists, then `t` must be a type such that `begin(inst)` is well-
    // -formed and returns an iterator compatible with `std::iterator_traits`, and the value type of
    // that iterator is tuple-like with at least two elements (where `inst` is an instance of `t`
    // and a `using std::begin` directive is visible).

    template<typename t>
    struct mapped_type {
        using type = typename impl::mapped_type::result<t, void>::type;
    };

    template<typename t>
    using mapped_type_t = typename mapped_type<t>::type;
}

#endif
