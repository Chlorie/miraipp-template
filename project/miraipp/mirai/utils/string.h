#pragma once

#include <string>
#include <utility>

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
}
