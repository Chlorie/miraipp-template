#include "message.h"

namespace mirai
{
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

    MessageChain plain_text(const std::string_view text) { return { { msg::Plain{ std::string(text) } } }; }

    namespace
    {
        constexpr std::array<std::string_view, std::variant_size_v<msg::Variant>> msg_types
        {
            "Source", "Quote", "At", "AtAll", "Face", "Plain",
            "Image", "FlashImage", "Xml", "Json", "App", "Poke"
        };

        template <size_t... I>
        void node_from_json_impl(const utils::json& json, MessageChainNode& value,
            std::index_sequence<I...>)
        {
            const std::string& type = json["type"].get_ref<const std::string&>();
            ((type == msg_types[I]
                  ? value = json.get<std::variant_alternative_t<I, msg::Variant>>()
                  : (void)0), ...);
        }
    }

    void to_json(utils::json& json, const MessageChainNode& value)
    {
        value.apply([&json](const auto& v) { json = v; });
    }

    void from_json(const utils::json& json, MessageChainNode& value)
    {
        node_from_json_impl(json, value,
            std::make_index_sequence<std::variant_size_v<msg::Variant>>{});
    }
}
