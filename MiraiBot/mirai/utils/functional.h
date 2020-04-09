#pragma once

#include <tuple>
#include <type_traits>

namespace mirai::utils
{
    /**
     * \brief Overload utility class to overload multiple functors
     * \tparam F Types of the functors
     */
    template <typename... F> struct Overload : F... { using F::operator()...; };
    template <typename... F> Overload(F...)->Overload<F...>;

    /**
     * \brief Provide a fallback function object for an overload,
     * usually used to ignore some cases when visiting a variant
     * \tparam T Type of the default return value
     * \param value The default return value
     * \return A lambda taking in anything, returning the given value
     */
    template <typename T>
    auto fallback(T&& value)
    {
        return[v = std::forward<T>(value)](...)->T
        {
            return std::forward<T>(v);
        };
    }

    /**
     * \brief Provide a fallback function object for an overload,
     * usually used to ignore some cases when visiting a variant
     * \return A lambda taking in anything, returning void
     */
    inline auto fallback() { return [](...) {}; }

    namespace detail
    {
        // One more layer of indirection to deal with moved-from tuple
        template <typename F, typename Tuple, typename... Ts>
        decltype(auto) bind_front_t_call_impl(F&& f, Tuple&& tup, Ts&&... args)
        {
            return std::apply([&f, &args...](auto&&... bound) -> decltype(auto)
            {
                return std::invoke(std::forward<F>(f),
                    std::forward<decltype(bound)>(bound)...,
                    std::forward<Ts>(args)...);
            }, std::forward<Tuple>(tup));
        }

        template <typename F, typename... Bs>
        class bind_front_t
        {
        private:
            F fd_;
            std::tuple<Bs...> tup_;
        public:
            template <typename G, typename... Ts>
            explicit bind_front_t(G&& fd, Ts&&... bound) :
                fd_(std::forward<G>(fd)), tup_(std::make_tuple(std::forward<Ts>(bound)...)) {}

            // invoke_result_t implies SFINAE so OK
            template <typename... Ts>
            auto operator()(Ts&&... args) &
                noexcept(std::is_nothrow_invocable_v<F&, Bs&..., Ts...>)
                -> std::invoke_result_t<F&, Bs&..., Ts...>
            {
                return bind_front_t_call_impl(fd_, tup_, std::forward<Ts>(args)...);
            }
            template <typename... Ts>
            auto operator()(Ts&&... args) const&
                noexcept(std::is_nothrow_invocable_v<const F&, const Bs&..., Ts...>)
                -> std::invoke_result_t<const F&, const Bs&..., Ts...>
            {
                return bind_front_t_call_impl(fd_, tup_, std::forward<Ts>(args)...);
            }
            template <typename... Ts>
            auto operator()(Ts&&... args) &&
                noexcept(std::is_nothrow_invocable_v<F, Bs..., Ts...>)
                -> std::invoke_result_t<F, Bs..., Ts...>
            {
                return bind_front_t_call_impl(std::move(fd_), std::move(tup_),
                    std::forward<Ts>(args)...);
            }
            template <typename... Ts>
            auto operator()(Ts&&... args) const&&
                noexcept(std::is_nothrow_invocable_v<const F, const Bs..., Ts...>)
                -> std::invoke_result_t<const F, const Bs..., Ts...>
            {
                return bind_front_t_call_impl(std::move(fd_), std::move(tup_),
                    std::forward<Ts>(args)...);
            }
        };

        // Deal with std::reference_wrapper
        template <typename T> struct unwrap { using type = T; };
        template <typename T> struct unwrap<std::reference_wrapper<T>> { using type = T&; };
        template <typename T> using make_tuple_elem_t = typename unwrap<std::decay_t<T>>::type;

        template <typename F, typename... Ts>
        bind_front_t(F, Ts...)->bind_front_t<std::decay_t<F>, make_tuple_elem_t<Ts>...>;
    }

    /**
     * \brief Generates a forwarding call wrapper which binds args as the leading parameters
     * \tparam F Type of the function objects
     * \tparam Args Types of the arguments to bind
     * \param f The function object to bind
     * \param args The arguments to bind
     * \return The result function object
     * \remarks This is a crude implementation for C++20's std::bind_front,
     * for more information please refer to cppreference.
     */
    template <typename F, typename... Args>
    auto bind_front(F&& f, Args&&... args)
    {
        return detail::bind_front_t(
            std::forward<F>(f),
            std::forward<Args>(args)...);
    }
}
