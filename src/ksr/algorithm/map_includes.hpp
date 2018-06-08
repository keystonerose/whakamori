#ifndef KSR_ALGORITHM_MAP_CONTAINS_HPP
#define KSR_ALGORITHM_MAP_CONTAINS_HPP

#include "../type_traits/container_traits.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <functional>
#include <type_traits>
#include <utility>
#include <vector>

namespace ksr {

    // Determines whether an associative container contains every element of the range
    // `[begin_keys, end_keys)` as a key.
    //
    // `t` must be an (iterable) container type, and `input_it` must be an input iterator type. The
    // following requirements must be satisfied:
    //
    // * `t` must provide the member function `const_iterator t::find(const key_type&)`, where
    //   `const_iterator` is the `const`-iterator type for `t` and `key_type` is its key type; and
    // * `input_it` must be dereferencable to `key_type`.

    template<typename t, typename input_it>
    auto includes_keys(const t& container, input_it begin_keys, input_it end_keys) -> bool {

        using std::end;

        return std::all_of(begin_keys, end_keys, [&container] (const auto& key) {
            return container.find(key) != end(container);
        });
    }

    // Determines whether a map-like (that is, key-value) associative container contains every
    // element of the sorted range `[begin_values, end_values)` as a mapped value. May throw if the
    // algorithm fails to allocate memory for temporary storage.
    //
    // `t` must be an (iterable) container type, and `input_it` must be an input iterator type. The
    // mapped type `mapped_type` of `t` must be deducible as if by `ksr::mapped_type`, and the
    // following requirements must be satisfied:
    //
    // * `mapped_type` must be copyable;
    // * `compare_fn` must be a less-than comparison function type on `mapped_type`; and
    // * `input_it` must be dereferencable to `mapped_type`.

    template<typename t, typename input_it, typename compare_fn>
    auto includes_mapped_values(
        const t& container, input_it begin_values, input_it end_values, compare_fn comp) -> bool {

        assert(std::is_sorted(begin_values, end_values, comp));

        using mapped_type  = mapped_type_t<t>;
        auto mapped_values = std::vector<mapped_type>{};
        mapped_values.reserve(container.size());

        for (const auto& elem : container) {
            mapped_values.push_back(std::get<1>(elem));
        }

        std::sort(mapped_values.begin(), mapped_values.end(), comp);
        return std::includes(mapped_values.begin(), mapped_values.end(), begin_values, end_values);
    }

    template<typename t, typename input_it>
    auto includes_mapped_values(
        const t& container, input_it begin_values, input_it end_values) -> bool {

        return includes_mapped_values(container, begin_values, end_values, std::less<>{});
    }
}

#endif
