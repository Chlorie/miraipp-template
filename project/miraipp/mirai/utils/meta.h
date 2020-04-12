#pragma once

#include <type_traits>

namespace mirai::utils
{
    /**
     * \brief Provide alias for the first type in a type pack
     * \tparam Ts The type pack
     */
    template <typename... Ts> struct first_type {};
    template <typename T, typename... Rs> struct first_type<T, Rs...> { using type = T; };

    /**
     * \brief Alias of the first type in a type pack
     * \tparam Ts The type pack
     */
    template <typename... Ts> using first_type_t = typename first_type<Ts...>::type;

    /**
     * \brief Provide a static bool constant for whether all of the types are the same
     * \tparam Ts The types to check
     */
    template <typename... Ts> struct same_type : std::false_type {};
    template <> struct same_type<> : std::true_type {};
    template <typename T, typename... Rs>
    struct same_type<T, Rs...> : std::bool_constant<(std::is_same_v<T, Rs>&& ...)> {};

    /**
     * \brief Check if all of the types are the same
     * \tparam Ts The types to check
     */
    template <typename... Ts> constexpr bool same_type_v = same_type<Ts...>::value;

    /**
     * \brief Alias for the same type if all the types are the same
     * \tparam Ts The type pack
     */
    template <typename... Ts>
    using same_type_t = std::enable_if_t<same_type_v<Ts...>, first_type_t<Ts...>>;

    /**
     * \brief Check whether a callable is suitable for visiting a variant with specific types
     * \tparam F Type of the callable object
     * \tparam Ts Type of the parameters
     */
    template <typename F, typename... Ts>
    constexpr bool is_visitable_v = same_type_v<std::invoke_result_t<F, Ts>...>;

    /**
     * \brief A type holding a list of types in its template parameters
     * \tparam Ts The type pack
     */
    template <typename... Ts> struct type_list {};

    /**
     * \brief Provide alias for applying a template to a type list
     * \tparam Templ The template to apply
     * \tparam List The type list
     */
    template <template <typename...> typename Templ, typename List> struct apply_template {};
    template <template <typename...> typename Templ, typename... Ts>
    struct apply_template<Templ, type_list<Ts...>> { using type = Templ<Ts...>; };

    /**
     * \brief Alias for applying a template to a type list
     * \tparam Templ The template to apply
     * \tparam List The type list
     */
    template <template <typename...> typename Templ, typename List>
    using apply_template_t = typename apply_template<Templ, List>::type;

    /**
     * \brief Provide alias to extract a type list from a variadic template type
     * \tparam T The type to extract the list from
     */
    template <typename T> struct extract_types {};
    template <template <typename...> typename Templ, typename... Ts>
    struct extract_types<Templ<Ts...>> { using type = type_list<Ts...>; };

    /**
     * \brief Alias to extract a type list from a variadic template type
     * \tparam T The type to extract the list from
     */
    template <typename T> using extract_types_t = typename extract_types<T>::type;

    /**
     * \brief Provide alias for concatenating two type lists
     * \tparam K The first type list
     * \tparam L The second type list
     */
    template <typename K, typename L> struct concat_list {};
    template <typename... Ts, typename... Us>
    struct concat_list<type_list<Ts...>, type_list<Us...>> { using type = type_list<Ts..., Us...>; };

    /**
     * \brief Alias for concatenating two type lists
     * \tparam K The first type list
     * \tparam L The second type list
     */
    template <typename K, typename L> using concat_list_t = typename concat_list<K, L>::type;

    /**
     * \brief Provide static bool member for whether the type list contains the type
     * \tparam L The type list
     * \tparam T The type
     */
    template <typename L, typename T> struct in_list : std::false_type {};
    template <typename... Ts, typename T>
    struct in_list<type_list<Ts...>, T> : std::bool_constant<(std::is_same_v<Ts, T> || ...)> {};

    /**
     * \brief Check whether the type list contains the type
     * \tparam L The type list
     * \tparam T The type
     */
    template <typename L, typename T> constexpr bool in_list_v = in_list<L, T>::value;
}
