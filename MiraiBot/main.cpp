#include <iostream>
#include "mirai/mirai.h"

int main()
{
    const char* auth_key = "somerandomauthkeyformybot";
    const int64_t qq = 3378448768;
    const int64_t dev = 1357008522;
    const int64_t dnd = 820594943;

    try
    {
        const mirai::Session sess(auth_key, qq);
        std::string result;
        for (const auto& group : sess.group_list())
            result += std::to_string(group.id) + ": " + group.name + '\n';
        sess.send_friend_message(dev, result);
    }
    catch (const mirai::RuntimeError& e)
    {
        std::cout << mirai::utils::utf8_to_local(e.what());
    }

    return 0;
}
