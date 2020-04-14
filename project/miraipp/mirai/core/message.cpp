#include "message.h"
#include <sstream>

namespace mirai
{
    namespace
    {
        constexpr std::array<std::string_view, std::variant_size_v<msg::Variant>> msg_types
        {
            "At", "AtAll", "Face", "Plain", "Image",
            "FlashImage", "Xml", "Json", "App", "Poke"
        };

        bool is_plain(const Segment& node) { return node.type() == SegmentType::plain; }

        std::string& get_text(Segment& node) { return node.get<msg::Plain>().text; }

        void combine_adjacent_text(MessageChain& chain)
        {
            size_t offset = 0;
            while (offset < chain.size())
            {
                const auto first = std::find_if(chain.begin() + offset, chain.end(), is_plain);
                if (first == chain.end()) return;
                const auto next = std::next(first);
                const auto last = std::find_if_not(next, chain.end(), is_plain);
                offset = size_t(last - chain.begin());
                std::string& text = get_text(*first);
                for (auto iter = next; iter != last; ++iter) text += get_text(*iter);
                chain.erase(next, last);
            }
        }
    }

    namespace msg
    {
        void to_json(utils::json& json, const Source& value)
        {
            json = {
                { "type", "Source" },
                { "id", value.id },
                { "time", value.time }
            };
        }

        void from_json(const utils::json& json, Source& value)
        {
            json["id"].get_to(value.id);
            json["time"].get_to(value.time);
        }

        void to_json(utils::json& json, const Quote& value)
        {
            json = {
                { "type", "Quote" },
                { "id", value.id },
                { "groupId", value.group_id },
                { "senderId", value.sender_id },
                { "targetId", value.target_id },
                { "origin", value.origin }
            };
        }

        void from_json(const utils::json& json, Quote& value)
        {
            json["id"].get_to(value.id);
            json["groupId"].get_to(value.group_id);
            json["senderId"].get_to(value.sender_id);
            json["targetId"].get_to(value.target_id);
            json["origin"].get_to(value.origin);
        }

        void to_json(utils::json& json, const At& value)
        {
            json = {
                { "type", "At" },
                { "target", value.target },
                { "display", value.display }
            };
        }

        void from_json(const utils::json& json, At& value)
        {
            json["target"].get_to(value.target);
            json["display"].get_to(value.display);
        }

        void to_json(utils::json& json, const AtAll& value)
        {
            json = {
                { "type", "AtAll" }
            };
        }

        void from_json(const utils::json&, AtAll&) {}

        void to_json(utils::json& json, const Face& value)
        {
            json = {
                { "type", "Face" },
                { "faceId", value.face_id },
                { "name", value.name }
            };
        }

        void from_json(const utils::json& json, Face& value)
        {
            json["faceId"].get_to(value.face_id);
            json["name"].get_to(value.name);
        }

        void to_json(utils::json& json, const Plain& value)
        {
            json = {
                { "type", "Plain" },
                { "text", value.text }
            };
        }

        void from_json(const utils::json& json, Plain& value)
        {
            json["text"].get_to(value.text);
        }

        void to_json(utils::json& json, const Image& value)
        {
            json = {
                { "type", "Image" },
                { "imageId", value.image_id },
                { "url", value.url },
                { "path", value.path }
            };
        }

        void from_json(const utils::json& json, Image& value)
        {
            json["imageId"].get_to(value.image_id);
            json["url"].get_to(value.url);
            json["path"].get_to(value.path);
        }

        void to_json(utils::json& json, const FlashImage& value)
        {
            json = {
                { "type", "Image" },
                { "imageId", value.image_id },
                { "url", value.url },
                { "path", value.path }
            };
        }

        void from_json(const utils::json& json, FlashImage& value)
        {
            json["imageId"].get_to(value.image_id);
            json["url"].get_to(value.url);
            json["path"].get_to(value.path);
        }

        void to_json(utils::json& json, const Xml& value)
        {
            json = {
                { "type", "Xml" },
                { "xml", value.xml }
            };
        }

        void from_json(const utils::json& json, Xml& value)
        {
            json["xml"].get_to(value.xml);
        }

        void to_json(utils::json& json, const Json& value)
        {
            json = {
                { "type", "Json" },
                { "json", value.json }
            };
        }

        void from_json(const utils::json& json, Json& value)
        {
            json["json"].get_to(value.json);
        }

        void to_json(utils::json& json, const App& value)
        {
            json = {
                { "type", "App" },
                { "content", value.content }
            };
        }

        void from_json(const utils::json& json, App& value)
        {
            json["content"].get_to(value.content);
        }

        void to_json(utils::json& json, const Poke& value)
        {
            json = {
                { "type", "Poke" },
                { "name", value.name }
            };
        }

        void from_json(const utils::json& json, Poke& value)
        {
            json["name"].get_to(value.name);
        }
    }

    // Implementation of Message

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

    // Implementation of StringifiedMessage

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

    namespace
    {
        template <size_t... I>
        void node_from_json_impl(const utils::json& json, Segment& value,
            std::index_sequence<I...>)
        {
            const std::string& type = json["type"].get_ref<const std::string&>();
            ((type == msg_types[I]
                  ? value = Segment(json.get<std::variant_alternative_t<I, msg::Variant>>())
                  : (void)0), ...);
        }
    }

    void to_json(utils::json& json, const Segment& value)
    {
        value.apply([&json](const auto& v) { json = v; });
    }

    void from_json(const utils::json& json, Segment& value)
    {
        node_from_json_impl(json, value,
            std::make_index_sequence<std::variant_size_v<msg::Variant>>{});
    }

    void to_json(utils::json& json, const Message& value) { json = value.chain(); }

    void from_json(const utils::json& json, ReceivedMessage& value)
    {
        MessageChain chain;
        for (auto& node : json)
        {
            if (node["type"] == "Source")
                value.source = node.get<msg::Source>();
            else if (node["type"] == "Quote")
                value.quote = node.get<msg::Quote>();
            else
                chain.emplace_back(node.get<Segment>());
        }
        value.content = std::move(chain);
    }
}
