#include "request.h"
#include <cpr/cpr.h>
#include "../core/common.h"

namespace mirai::utils
{
    std::string get_no_parse(const std::string_view url, const cpr::Parameters& parameters)
    {
        const cpr::Response response = Get(
            cpr::Url{ std::string(base_url) += url }, parameters);
        if (response.status_code != 200) // Status code not OK
            throw RuntimeError(response.error.message);
        return response.text;
    }

    json get(const std::string_view url, const cpr::Parameters& parameters)
    {
        return json::parse(get_no_parse(url, parameters));
    }

    std::string post_json_no_parse(const std::string_view url, const json& json)
    {
        const cpr::Response response = Post(cpr::Url{ std::string(base_url) += url },
            cpr::Header{ { "Content-Type", "application/json; charset=utf-8" } },
            cpr::Body{ json.dump() });
        if (response.status_code != 200) // Status code not OK
            throw RuntimeError(response.error.message);
        return response.text;
    }

    json post_json(const std::string_view url, const json& json)
    {
        return json::parse(post_json_no_parse(url, json));
    }

    void check_response(const json& json)
    {
        const auto iter = json.find("code");
        if (iter != json.end() && iter->get<int32_t>() != 0)
            throw RuntimeError(json["msg"].get_ref<const std::string&>());
    }
}
