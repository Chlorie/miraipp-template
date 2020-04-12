#include "common.h"
#include <iostream>
#include "../utils/encoding.h"

namespace mirai
{
    void error_logger(const RuntimeError& e)
    {
        std::cerr << utils::utf8_to_local(e.what()) << '\n';
    }

    void error_rethrower(const RuntimeError& e) { throw e; }
}
