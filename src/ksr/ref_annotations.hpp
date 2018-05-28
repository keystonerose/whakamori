#ifndef KSR_REF_ANNOTATIONS_HPP
#define KSR_REF_ANNOTATIONS_HPP

#include <functional>
#include <memory>
#include <type_traits>

namespace ksr {

    // Annotation classes for function parameters, to make explicit at the call site any special
    // semantic considerations for reference argument. A function taking a reference parameter may
    // consider using such an annotation to indicate that that argument may be mutated by the call,
    // or that a persistent reference to it is stored for longer than the duration of the call (and
    // that the caller must therefore make certain lifetime guarantees).
    //
    // These differ from `std::reference_wrapper` in several respects.
    // * Explicit construction is required, so that the annotation is visible at the call site.
    // * There is no special integration of these types with the standard library; accordingly, no
    //   function call operator is provided.

    namespace ref_annotations {

        // Annotation to indicate that a reference parameter for a function is persistently stored
        // for longer than the duration of the function call, and that it may be mutated by the
        // callee during that time. The caller must be acknowledge the possibility of mutation and
        // be attentive to lifetime requirements of the referenced object.

        template<typename t>
        class mut_observer {
        public:

            static_assert(!std::is_const_v<t>);

            explicit mut_observer(t& obj) noexcept : _ptr{std::addressof(obj)} {}
            explicit mut_observer(t&&) = delete;

            mut_observer(mut_observer&&)      noexcept = default;
            mut_observer(const mut_observer&) noexcept = default;

            auto operator=(mut_observer&&)      -> mut_observer& = delete;
            auto operator=(const mut_observer&) -> mut_observer& = delete;

            operator t&() const noexcept { return get(); }

            auto get() const noexcept -> t& { return *_ptr; }
            auto ref() const noexcept -> std::reference_wrapper<t> { return *_ptr; }

        private:

            t* _ptr; // Non-null at all times
        };

        // Annotation to indicate that a reference parameter for a function is persistently stored
        // for longer than the duration of the function call. The caller must be attentive to
        // lifetime requirements of the referenced object.

        template<typename t>
        class observer {
        public:

            explicit observer(const t&        obj) noexcept : _ptr{std::addressof(obj)}       {}
            explicit observer(mut_observer<t> obj) noexcept : _ptr{std::addressof(obj.get())} {}
            explicit observer(t&&) = delete;

            observer(observer&&)      noexcept = default;
            observer(const observer&) noexcept = default;

            auto operator=(observer&&)      -> observer& = delete;
            auto operator=(const observer&) -> observer& = delete;

            operator const t&() const noexcept { return get(); }

            auto get() const noexcept -> const t& { return *_ptr; }
            auto ref() const noexcept -> std::reference_wrapper<const t> { return *_ptr; }

        private:

            const t* _ptr; // Non-null at all times
        };

        // Annotation to indicate that a reference parameter for a function may be mutated by the
        // call. The caller must be acknowledge the possibility of this mutation.

        template<typename t>
        class mut {
        public:

            static_assert(!std::is_const_v<t>);

            explicit mut(t&              obj) noexcept : _ptr{std::addressof(obj)}       {}
            explicit mut(mut_observer<t> obj) noexcept : _ptr{std::addressof(obj.get())} {}
            explicit mut(t&&) = delete;

            mut(mut&&)      noexcept = default;
            mut(const mut&) noexcept = default;

            auto operator=(mut&&)      -> mut& = delete;
            auto operator=(const mut&) -> mut& = delete;

            operator t&() const noexcept { return get(); }

            auto get() const noexcept -> t& { return *_ptr; }
            auto ref() const noexcept -> std::reference_wrapper<t> { return *_ptr; }

        private:

            t* _ptr; // Non-null at all times
        };
    }

    using namespace ref_annotations;
}

#endif
