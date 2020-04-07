#pragma once

#include <string>
#include <stdexcept>

namespace mirai
{
    inline std::string base_url = "localhost:8080";

    class RuntimeError : public std::runtime_error { using std::runtime_error::runtime_error; };
}
