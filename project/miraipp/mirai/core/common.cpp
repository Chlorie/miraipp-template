#include "common.h"
#include <iostream>
#include <cpr/parameters.h>
#include "../utils/encoding.h"
#include "../utils/request.h"

namespace mirai
{
    std::string get_version()
    {
        const auto res = utils::get("/about", cpr::Parameters{});
        utils::check_response(res);
        return res["data"]["version"].get<std::string>();
    }

    void error_logger()
    {
        try { throw; }
        catch (const RuntimeError& e)
        {
            std::cerr << "[Mirai Runtime Error] " << utils::utf8_to_local(e.what()) << '\n';
        }
        catch (const std::runtime_error& e)
        {
            std::cerr << "[Runtime Error] " << e.what() << '\n';
        }
        catch (const std::exception& e)
        {
            std::cerr << "[Exception] " << e.what() << '\n';
        }
        catch (...)
        {
            std::cerr << "[Unknown Error]\n";
        }
    }
}
