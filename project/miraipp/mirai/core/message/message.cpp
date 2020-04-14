#include "message.h"
#include <sstream>
#include "common.h"
#include "../common.h"

namespace mirai
{
    Message::Message(MessageChain chain): chain_(std::move(chain))
    {
        combine_adjacent_text(chain_);
    }

    Message::Message(Segment node): chain_{ std::move(node) } {}

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

    Message& Message::operator=(const std::string_view plain_text)
    {
        chain_ = { { msg::Plain{ std::string(plain_text) } } };
        return *this;
    }

    void Message::push_back(Segment node)
    {
        if (is_plain(node) && !empty() && is_plain(chain_.back()))
            get_plain(chain_.back()) += get_plain(node);
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

    Message& Message::operator+=(const std::string_view plain_text)
    {
        return emplace_back(msg::Plain{ std::string(plain_text) });
    }

    Message operator+(Message lhs, Segment rhs) { return lhs += std::move(rhs); }

    bool operator==(const Message& lhs, const std::string_view rhs)
    {
        if (lhs.chain_.size() != 1 || !is_plain(lhs.chain_.front())) return false;
        return get_plain(lhs.chain_.front()) == rhs;
    }

    std::string Message::extract_text() const
    {
        std::string res;
        for (auto& node : chain_)
            node.dispatch([&res](const msg::Plain& plain) { res += plain.text; });
        return res;
    }

    std::string Message::stringify() const
    {
        std::ostringstream oss;
        for (const auto& segment : chain_)
            oss << segment.apply([](const auto& seg) { return seg.stringify(); });
        return oss.str();
    }

    bool Message::starts_with(const std::string_view text) const
    {
        if (chain_.empty() || !is_plain(chain_.front())) return false;
        const std::string_view prefix = std::string_view(get_plain(chain_.front()))
           .substr(0, text.size());
        return prefix == text;
    }

    bool Message::ends_with(const std::string_view text) const
    {
        if (chain_.empty() || !is_plain(chain_.back())) return false;
        const std::string_view last = std::string_view(get_plain(chain_.back()));
        if (last.size() < text.size()) return false;
        const std::string_view suffix = last.substr(last.size() - text.size());
        return suffix == text;
    }

    bool Message::contains(const std::string_view text) const
    {
        return std::any_of(chain_.begin(), chain_.end(),
            [text](const Segment& seg)
            {
                return is_plain(seg)
                    && (get_plain(seg).find(text) != std::string::npos);
            });
    }

    std::string Message::escape(const std::string_view unescaped)
    {
        size_t offset = 0;
        std::string result;
        result.reserve(unescaped.size());
        while (true)
        {
            const size_t pos = unescaped.find_first_of("\\[]{}", offset);
            if (pos == std::string::npos) break;
            const size_t length = pos - offset;
            result += unescaped.substr(offset, length);
            switch (unescaped[pos])
            {
                case '\\':
                    result += "\\\\";
                    break;
                case '[':
                    result += "\\[";
                    break;
                case ']':
                    result += "\\]";
                    break;
                case '{':
                    result += "[[";
                    break;
                case '}':
                    result += "]]";
                    break;
                default: break;
            }
            offset = pos + 1;
        }
        result += unescaped.substr(offset);
        return result;
    }

    std::string Message::unescape(const std::string_view escaped)
    {
        const char* error = "Erroneous escaped string";
        size_t offset = 0;
        std::string result;
        result.reserve(escaped.size());
        while (true)
        {
            const size_t pos = escaped.find_first_of("\\[", offset);
            if (pos == std::string::npos) break;
            const size_t length = pos - offset;
            if (length == escaped.size() - 1) throw RuntimeError(error);
            result += escaped.substr(offset, length);
            if (escaped[pos] == '[')
            {
                if (escaped[pos + 1] != '[') throw RuntimeError(error);
                result += '{';
            }
            else if (escaped[pos] == ']')
            {
                if (escaped[pos + 1] != ']') throw RuntimeError(error);
                result += '}';
            }
            else // escaped[pos] == '\\'
            {
                const char next = escaped[pos + 1];
                if (next != '\\' && next != '[' && next != ']') throw RuntimeError(error);
                result += next;
            }
            offset = pos + 2;
        }
        result += escaped.substr(offset);
        return result;
    }
}
