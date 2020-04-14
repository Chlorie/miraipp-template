#include "message.h"
#include "common.h"
#include "stringified_message.h"

namespace mirai
{
    Message::Message(MessageChain chain): chain_(std::move(chain))
    {
        combine_adjacent_text(chain_);
    }

    Message::Message(Segment node): chain_{ std::move(node) } {}

    Message::Message(StringifiedMessage message): chain_(std::move(message).chain()) {}

    Message::Message(const std::string_view plain_text):
        chain_({ msg::Plain{ std::string(plain_text) } }) {}

    Message& Message::operator=(MessageChain chain)
    {
        chain_ = std::move(chain);
        combine_adjacent_text(chain_);
        return *this;
    }

    Message& Message::operator=(Segment node)
    {
        chain_ = MessageChain{ std::move(node) };
        return *this;
    }

    Message& Message::operator=(StringifiedMessage message)
    {
        chain_ = std::move(message).chain();
        return *this;
    }

    Message& Message::operator=(const std::string_view plain_text)
    {
        chain_ = { { msg::Plain{ std::string(plain_text) } } };
        return *this;
    }

    void Message::push_back(Segment node)
    {
        if (is_plain(node) && !empty() && is_plain(chain_.back()))
            get_text(chain_.back()) += get_text(node);
        else
            chain_.emplace_back(std::move(node));
    }

    Message& Message::emplace_back(Segment node)
    {
        push_back(std::move(node));
        return *this;
    }

    Message& Message::operator+=(const Message& message)
    {
        if (message.empty()) return *this;
        push_back(message.chain_.front());
        chain_.insert(chain_.end(),
            std::next(message.chain_.begin()),
            message.chain_.end());
        return *this;
    }

    Message& Message::operator+=(Message&& message)
    {
        if (message.empty()) return *this;
        push_back(message.chain_.front());
        chain_.insert(chain_.end(),
            std::make_move_iterator(std::next(message.chain_.begin())),
            std::make_move_iterator(message.chain_.end()));
        return *this;
    }

    Message& Message::operator+=(Segment node) { return emplace_back(std::move(node)); }

    Message operator+(Message lhs, Segment rhs) { return lhs += std::move(rhs); }

    Message& Message::operator+=(const std::string_view plain_text)
    {
        return emplace_back(msg::Plain{ std::string(plain_text) });
    }
}
