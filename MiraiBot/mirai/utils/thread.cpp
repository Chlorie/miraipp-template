#include "thread.h"

namespace mirai::utils
{
    void Thread::force_join() noexcept
    {
        if (thread_.joinable())
        {
            try { thread_.join(); }
            catch (...) { std::abort(); }
        }
    }

    Thread& Thread::operator=(Thread&& other) noexcept
    {
        Thread copy(std::move(other));
        swap(copy);
        return *this;
    }
}
