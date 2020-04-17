#include "received_message.h"

namespace mirai
{
    void from_json(const utils::json& json, ReceivedMessage& value)
    {
        MessageChain chain;
        for (auto& node : json)
        {
            if (node.at("type") == "Source")
                value.source = node.get<msg::Source>();
            else if (node.at("type") == "Quote")
                value.quote = node.get<msg::Quote>();
            else
                chain.emplace_back(node.get<Segment>());
        }
        if (value.quote) chain.erase(chain.begin()); // Quote messages contains an extra At
        value.content = std::move(chain);
    }
}
