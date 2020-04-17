#include "segment.h"
#include <sstream>
#include "common.h"

namespace mirai
{
    namespace msg
    {
        std::string At::stringify() const
        {
            std::ostringstream oss;
            oss << "{at:" << target << '}';
            return oss.str();
        }

        std::string Face::stringify() const
        {
            std::ostringstream oss;
            oss << "{face:";
            if (face_id) oss << *face_id;
            else oss << '?';
            oss << '}';
            return oss.str();
        }

        std::string Image::stringify() const
        {
            std::ostringstream oss;
            oss << "{image:";
            if (image_id) oss << Message::escape(*image_id);
            else oss << '?';
            oss << '}';
            return oss.str();
        }

        std::string FlashImage::stringify() const
        {
            std::ostringstream oss;
            oss << "{flash_image:";
            if (image_id) oss << Message::escape(*image_id);
            else oss << '?';
            oss << '}';
            return oss.str();
        }

        std::string Xml::stringify() const
        {
            std::ostringstream oss;
            oss << "{xml:" << Message::escape(xml) << '}';
            return oss.str();
        }

        std::string Json::stringify() const
        {
            std::ostringstream oss;
            oss << "{json:" << Message::escape(json) << '}';
            return oss.str();
        }

        std::string App::stringify() const
        {
            std::ostringstream oss;
            oss << "{app:" << Message::escape(content) << '}';
            return oss.str();
        }

        std::string Poke::stringify() const
        {
            std::ostringstream oss;
            oss << "{poke:" << name << '}';
            return oss.str();
        }

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
            json.at("id").get_to(value.id);
            json.at("time").get_to(value.time);
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
            json.at("id").get_to(value.id);
            json.at("groupId").get_to(value.group_id);
            json.at("senderId").get_to(value.sender_id);
            json.at("targetId").get_to(value.target_id);
            json.at("origin").get_to(value.origin);
            // This is an issue where there's an At segment with target = 0 at the beginning
            if (!value.origin.empty())
            {
                auto& chain = value.origin.chain();
                if (chain.front().type() == SegmentType::at)
                {
                    const At& at = chain.front().get<At>();
                    if (at.target == 0) chain.erase(chain.begin());
                }
            }
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
            json.at("target").get_to(value.target);
            json.at("display").get_to(value.display);
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
            json.at("faceId").get_to(value.face_id);
            json.at("name").get_to(value.name);
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
            json.at("text").get_to(value.text);
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
            json.at("imageId").get_to(value.image_id);
            json.at("url").get_to(value.url);
            json.at("path").get_to(value.path);
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
            json.at("imageId").get_to(value.image_id);
            json.at("url").get_to(value.url);
            json.at("path").get_to(value.path);
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
            json.at("xml").get_to(value.xml);
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
            json.at("json").get_to(value.json);
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
            json.at("content").get_to(value.content);
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
            json.at("name").get_to(value.name);
        }
    }

    namespace
    {
        template <size_t... I>
        void node_from_json_impl(const utils::json& json, Segment& value,
            std::index_sequence<I...>)
        {
            const std::string& type = json.at("type").get_ref<const std::string&>();
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

    void from_json(const utils::json& json, Message& value) { json.get_to(value.chain()); }
}
