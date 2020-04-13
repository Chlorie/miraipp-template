#include <iostream>
#include <mirai/mirai.h>

int main()
{
    const char* auth_key = "somerandomauthkeyformybot";
    const int64_t qq = 3378448768;

    try
    {
        mirai::Session sess(auth_key, qq);
        sess.config(mirai::SessionConfig{ 4096, true });
        sess.start_websocket_client();
        auto& connection = sess.subscribe_all_events([&](const mirai::Event& event)
        {
            event.dispatch([&](const mirai::GroupRecallEvent& e)
            {
                const auto& msg = sess.message_from_id(e.message_id)
                                      .get<mirai::GroupMessage>()
                                      .message;
                std::string str;
                for (auto& node : msg)
                    node.dispatch([&](const mirai::msg::Plain& plain) { str += plain.text; });
                sess.send_group_message(e.group.id, str, e.message_id);
            });
        }, mirai::error_logger);
        std::cin.get();
        if (connection.ended())
            throw mirai::RuntimeError(connection.error_reason());
    }
    catch (const mirai::RuntimeError& e)
    {
        std::cerr << mirai::utils::utf8_to_local(e.what()) << '\n';
    }
}
