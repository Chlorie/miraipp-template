#include "common.h"
#include <iostream>
#include "../utils/encoding.h"

namespace mirai
{
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
