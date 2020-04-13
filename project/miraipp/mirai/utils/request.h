#pragma once

#include <nlohmann/json.hpp>

namespace cpr
{
    class Parameters;
}

namespace mirai::utils
{
    using json = nlohmann::json;

    /**
     * \brief GET request, throw if status code is not 200 (OK)
     * \param url The URL, relative to base_url
     * \param parameters The parameters
     * \return The text part of the response
     */
    std::string get_no_parse(std::string_view url, const cpr::Parameters& parameters);

    /**
     * \brief GET request, throw if status code is not 200 (OK), parse text into json
     * \param url The URL, relative to base_url
     * \param parameters The parameters
     * \return The text part of the response, parsed into json
     */
    json get(std::string_view url, const cpr::Parameters& parameters);

    /**
     * \brief POST request, throw if status code is not 200 (OK)
     * \param url The URL, relative to base_url
     * \param json The parameters
     * \return The text part of the response
     */
    std::string post_json_no_parse(std::string_view url, const json& json);

    /**
     * \brief POST request, throw if status code is not 200 (OK), parse text into json
     * \param url The URL, relative to base_url
     * \param json The parameters
     * \return The text part of the response, parsed into json
     */
    json post_json(std::string_view url, const json& json);

    /**
     * \brief Check return code from mirai HTTP API in the response json object,
     * throw if the code is not 0 (success)
     * \param json The response json object
     */
    void check_response(const json& json);
}
