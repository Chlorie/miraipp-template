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
        const int32_t msgid = sess.send_group_message(dnd, mirai::plain_text("Hello"));
        sess.send_group_message(dnd, mirai::plain_text("World"), msgid);
    }
    catch (const mirai::RuntimeError& e)
    {
        std::cout << mirai::utils::utf8_to_local(e.what());
    }

    return 0;
}
