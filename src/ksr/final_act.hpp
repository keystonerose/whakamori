#ifndef KSR_FINAL_ACT_HPP
#define KSR_FINAL_ACT_HPP

#include <functional>
#include <type_traits>

namespace ksr {

    // Scope guard that ensures that a specified callback function is executed at the end of a
    // scope. `f` must be a callable type meeting the following requirements:
    // * invocable with the signature `void()`;
    // * non-throwing (though not necessarily `noexcept`) when invoked; and
    // * move-constructible with the strong exception-safety guarantee.

    template<typename f>
    class final_act {
    public:

        static_assert(std::is_invocable_v<f>);

        explicit final_act(const f& func)
        try : _func{func} {}
        catch (...) {
            std::invoke(func);
            throw;
        }

        explicit final_act(f&& func)
        try : _func{std::move(func)} {}
        catch (...) {
            std::invoke(func);
            throw;
        }

        ~final_act() {
            std::invoke(_func);
        }

        final_act(final_act&&)      = delete;
        final_act(const final_act&) = delete;

        auto operator=(final_act&&)      -> final_act& = delete;
        auto operator=(const final_act&) -> final_act& = delete;

    private:

        f _func;
    };
}

#endif
