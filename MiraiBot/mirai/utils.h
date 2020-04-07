#pragma once

#include <nlohmann/json.hpp>
#include <optional>

namespace cpr { class Parameters; }

// For serializing standard library types
namespace nlohmann
{
    template <typename T>
    struct adl_serializer<std::optional<T>>
    {
        static void to_json(json& json, const std::optional<T>& value)
        {
            if (value.has_value())
                json = *value;
            else
                json = nullptr;
        }

        static void from_json(const json& json, std::optional<T>& value)
        {
            if (json.is_null())
                value = std::nullopt;
            else
                value = json.get<T>();
        }
    };
}

namespace mirai::utils
{
    using nlohmann::json;

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

    std::string get_no_parse(std::string_view url, const cpr::Parameters& parameters);
    json get(std::string_view url, const cpr::Parameters& parameters);

    std::string post_json_no_parse(std::string_view url, const json& json);
    json post_json(std::string_view url, const json& json);
    void check_response(const json& json);

    std::string convert_encoding(std::string_view text,
        const char* src_enc, const char* dst_enc, float capability_factor = 2.0f);

    std::string local_to_utf8(std::string_view text);
    std::string utf8_to_local(std::string_view text);
}
