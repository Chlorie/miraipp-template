#pragma once

#include <string>
#include <stdexcept>

namespace mirai
{
    /**
     * \brief Base URL of all the HTTP requests
     */
    inline std::string base_url = "localhost:8080";

    /**
     * \brief Exception class for runtime errors in mirai API
     */
    class RuntimeError : public std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

    /**
     * \brief Get the version of the Mirai HTTP API plugin
     * \return The version string
     */
    std::string get_version();

    /**
     * \brief A simple error handler which logs every error to the console
     */
    void error_logger();

    /**
     * \brief A simple error handler which rethrows every error
     */
    inline void error_rethrower() { throw; }
}
