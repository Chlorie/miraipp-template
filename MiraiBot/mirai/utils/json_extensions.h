#pragma once

#include <optional>
#include <nlohmann/json.hpp>

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

namespace mirai::utils { using nlohmann::json; }
