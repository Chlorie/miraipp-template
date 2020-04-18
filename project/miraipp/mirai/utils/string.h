#pragma once

#include <string>
#include <optional>
#include <array>
#include <charconv>

namespace mirai::utils
{
    /**
     * \brief Generic string length getter
     * \tparam N The length of the C character array
     * \param c_array The C-style array
     * \return Length of the string (length of the array minus 1)
     */
    template <size_t N> size_t strlen(const char (&c_array)[N]) { return N - 1; }

    /**
     * \brief Generic string length getter
     * \param c_str The C-style string (const char*)
     * \return Length of the string (calling std::strlen)
     */
    inline size_t strlen(const char* c_str) { return std::strlen(c_str); }

    /**
     * \brief Generic string length getter
     * \tparam T The type of the string like object
     * \param str The string like object
     * \return Length of the string (calling .length() method)
     */
    template <typename T> size_t strlen(const T& str) { return str.length(); }

    namespace detail
    {
        template <typename... Ts, size_t... Idx>
        std::string strcat_impl(std::index_sequence<Idx...>, const Ts&... strs)
        {
            const std::array lengths{ strlen(strs)... };
            const size_t total = (lengths[Idx] + ...);
            std::string result(total, '\0');
            size_t accumulate = 0;
            const auto copy = [&lengths, &result, &accumulate](const size_t index, const auto& str)
            {
                std::memcpy(&result[accumulate], &str[0], lengths[index]);
                accumulate += lengths[index];
            };
            (copy(Idx, strs), ...);
            return result;
        }
    }

    /**
     * \brief Concatenate several string like objects (std::string, const char*, etc.)
     * \tparam Ts Types of the string like objects
     * \param strs The strings
     * \return The concatenated string
     * \remarks This function allocates only once, better than using several operator+.
     */
    template <typename... Ts>
    std::string strcat(const Ts&... strs)
    {
        return detail::strcat_impl(std::index_sequence_for<Ts...>(), strs...);
    }

    /**
     * \brief Provides a static parse function for parsing strings
     * \tparam T The type to parse to
     * \remarks Specialize this class to support other types
     */
    template <typename T, typename = void> struct Parser {};

    template <typename T>
    struct Parser<T, std::enable_if_t<std::is_arithmetic_v<T>>>
    {
        static T parse(const std::string_view sv)
        {
            T result;
            const char* begin = sv.data();
            const char* end = begin + sv.size();
            const auto [ptr, ec] = std::from_chars(begin, end, result);
            if (ec == std::errc::result_out_of_range)
                throw std::runtime_error("Result out of range");
            if (ec == std::errc::invalid_argument || ptr != end)
                throw std::runtime_error("Invalid argument");
            return result;
        }
    };

    template <typename T>
    struct Parser<std::optional<T>, void>
    {
        static std::optional<T> parse(const std::string_view sv)
        {
            T result;
            const char* begin = sv.data();
            const char* end = begin + sv.size();
            const auto [ptr, ec] = std::from_chars(begin, end, result);
            if (ec != std::errc{} || ptr != end) return {};
            return result;
        }
    };

    template <>
    struct Parser<std::string, void>
    {
        static std::string parse(const std::string_view sv) { return std::string(sv); }
    };

    template <>
    struct Parser<std::string_view, void>
    {
        static std::string_view parse(const std::string_view sv) { return sv; }
    };

    namespace detail
    {
        template <typename T, typename = void> struct parsable : std::false_type {};
        template <typename T> struct parsable<T, std::void_t<decltype(&Parser<T>::parse)>> : std::true_type {};
        template <typename T> constexpr bool parsable_v = parsable<T>::value;

        template <typename T> constexpr bool parsable_or_void_v = parsable<T>::value || std::is_same_v<T, void>;
    }

    /**
     * \brief Parse a string to a specific type
     * \tparam T The type to parse to
     * \param sv The string view to parse
     * \return The result
     * \remarks Default parser provides parse functions for all arithmetic
     * types (throw if parsing fails), std::optional of arithmetic types
     * (return null if parsing fails), std::string (construct a string)
     * and std::string_view (do nothing).
     * Specialize the Parser class to support more types.
     */
    template <typename T, std::enable_if_t<detail::parsable_v<T>>* = nullptr>
    auto parse(const std::string_view sv) { return Parser<T>::parse(sv); }

    namespace detail
    {
        template <typename T>
        struct tag
        {
            using type = T;
        };

        template <size_t Rest, typename... Ts> struct nth_impl {};
        template <typename T, typename... Ts>
        struct nth_impl<0, T, Ts...>
        {
            using type = T;
        };
        template <size_t Rest, typename T, typename... Ts>
        struct nth_impl<Rest, T, Ts...>
        {
            using type = typename nth_impl<Rest - 1, Ts...>::type;
        };
        template <size_t N, typename... Ts> using nth_type = typename nth_impl<N, Ts...>::type;

        template <typename... Ts> constexpr size_t non_void_count = ((size_t(!std::is_same_v<Ts, void>)) + ... + 0);

        template <typename... Ts>
        constexpr auto non_void_indices()
        {
            std::array<size_t, non_void_count<Ts...>> res{};
            size_t type_index = 0, array_index = 0;
            const auto process = [&](auto tag)
            {
                using T = typename decltype(tag)::type;
                if constexpr (!std::is_void_v<T>) res[array_index++] = type_index;
                type_index++;
            };
            (process(tag<Ts>{}), ...);
            return res;
        }

        template <typename Sub>
        std::string_view submatch_to_sv(Sub submatch)
        {
            return std::string_view(&*submatch.first, submatch.length());
        }

        template <typename... Ts, typename T, size_t... Idx>
        auto parse_captures_impl(const T& match, std::index_sequence<Idx...>)
        {
            static constexpr auto indices = non_void_indices<Ts...>();
            return std::tuple{ parse<nth_type<indices[Idx], Ts...>>(submatch_to_sv(match[indices[Idx]]))... };
        }
    }

    /**
     * \brief Parse capture groups of a regex match
     * \tparam Ts The types to parse to, void for skipping that capture group
     * \param match The regex match type, for example std::smatch
     * \return Parsed results as a tuple
     * \remarks The types correspond to the capture groups
     */
    template <typename... Ts, typename T>
    auto parse_captures(const T& match,
        std::enable_if_t<(detail::parsable_or_void_v<Ts> && ...)>* = nullptr)
    {
        return detail::parse_captures_impl<Ts...>(match,
            std::make_index_sequence<detail::non_void_count<Ts...>>{});
    }
}
