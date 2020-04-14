#include "stringified_message.h"
#include <sstream>
#include "common.h"

namespace mirai
{
    void StringifiedMessage::stringify()
    {
        std::ostringstream oss;
        for (size_t i = 0; i < chain_.size(); i++)
        {
            auto& node = chain_[i];
            if (node.type() == SegmentType::plain)
                oss << node.get<msg::Plain>().text;
            else
            {
                oss << '\\' << msg_types[size_t(node.type())]
                    << ':' << i;
            }
        }
        string_ = oss.str();
    }

    StringifiedMessage::StringifiedMessage(MessageChain chain): chain_(std::move(chain))
    {
        combine_adjacent_text(chain_);
        stringify();
    }

    StringifiedMessage& StringifiedMessage::operator=(MessageChain chain)
    {
        chain_ = std::move(chain);
        combine_adjacent_text(chain_);
        stringify();
        return *this;
    }

    MessageChain StringifiedMessage::chain() &&
    {
        string_.clear();
        return std::move(chain_);
    }

    std::string StringifiedMessage::extract_text() const
    {
        std::string res;
        for (auto& node : chain_)
            node.dispatch([&res](const msg::Plain& plain) { res += plain.text; });
        return res;
    }
}
