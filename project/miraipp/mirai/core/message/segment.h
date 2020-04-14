#pragma once

#include "message.h"
#include "../../utils/json_extensions.h"
#include "../../utils/variant_wrapper.h"

namespace mirai
{
    namespace msg
    {
        /**
         * \brief The source of a message
         */
        struct Source final
        {
            int32_t id = 0; ///< The message ID of this message
            int32_t time = 0; ///< The timestamp when this message was sent
            friend bool operator==(const Source& lhs, const Source& rhs)
            {
                return lhs.id == rhs.id
                    && lhs.time == rhs.time;
            }
            friend bool operator!=(const Source& lhs, const Source& rhs) { return !(lhs == rhs); }
        };

        /**
         * \brief Quoting a message
         */
        struct Quote final
        {
            int32_t id = 0; ///< The ID of the message being quoted
            int64_t group_id = 0; ///< The group from which the quoted message is sent (group message)
            int64_t sender_id = 0; ///< The sender of the quoted message (friend message)
            int64_t target_id = 0; ///< The target of the quoted message (group ID or user ID)
            MessageChain origin; ///< The original quoted message
            friend bool operator==(const Quote& lhs, const Quote& rhs)
            {
                return lhs.id == rhs.id
                    && lhs.group_id == rhs.group_id
                    && lhs.sender_id == rhs.sender_id
                    && lhs.target_id == rhs.target_id;
            }
            friend bool operator!=(const Quote& lhs, const Quote& rhs) { return !(lhs == rhs); }
        };

        /**
         * \brief Mentioning someone
         */
        struct At final
        {
            int32_t target = 0; ///< Mentioned group member ID
            std::string display; ///< The string for display the @ message
            friend bool operator==(const At& lhs, const At& rhs) { return lhs.target == rhs.target; }
            friend bool operator!=(const At& lhs, const At& rhs) { return !(lhs == rhs); }
        };

        /**
         * \brief Mentioning everyone
         */
        struct AtAll final
        {
            friend bool operator==(AtAll, AtAll) { return true; }
            friend bool operator!=(AtAll, AtAll) { return false; }
        };

        /**
         * \brief QQ emoji
         */
        struct Face final
        {
            std::optional<int32_t> face_id = 0; ///< The ID of the emoji
            std::optional<std::string> name; ///< The name of the emoji
            friend bool operator==(const Face& lhs, const Face& rhs)
            {
                if (lhs.face_id && rhs.face_id) return lhs.face_id == rhs.face_id;
                return lhs.name == rhs.name;
            }
            friend bool operator!=(const Face& lhs, const Face& rhs) { return !(lhs == rhs); }
        };

        /**
         * \brief Plain text message segment
         */
        struct Plain final
        {
            std::string text;
            friend bool operator==(const Plain& lhs, const Plain& rhs) { return lhs.text == rhs.text; }
            friend bool operator!=(const Plain& lhs, const Plain& rhs) { return !(lhs == rhs); }
        };

        /**
         * \brief An image
         */
        struct Image final
        {
            std::optional<std::string> image_id; ///< The ID of the image
            std::optional<std::string> url; ///< The URL of the image
            std::optional<std::string> path; ///< The relative path to "plugins/MiraiAPIHTTP/images" of a local image
            friend bool operator==(const Image& lhs, const Image& rhs)
            {
                if (lhs.image_id && rhs.image_id) return lhs.image_id == rhs.image_id;
                if (lhs.url && rhs.url) return lhs.url == rhs.url;
                return lhs.path == rhs.path;
            }
            friend bool operator!=(const Image& lhs, const Image& rhs) { return !(lhs == rhs); }
        };

        /**
         * \brief A flash image
         */
        struct FlashImage final
        {
            std::optional<std::string> image_id; ///< The ID of the image
            std::optional<std::string> url; ///< The URL of the image
            std::optional<std::string> path; ///< The relative path to "plugins/MiraiAPIHTTP/images" of a local image
            friend bool operator==(const FlashImage& lhs, const FlashImage& rhs)
            {
                if (lhs.image_id && rhs.image_id) return lhs.image_id == rhs.image_id;
                if (lhs.url && rhs.url) return lhs.url == rhs.url;
                return lhs.path == rhs.path;
            }
            friend bool operator!=(const FlashImage& lhs, const FlashImage& rhs) { return !(lhs == rhs); }
        };

        /**
         * \brief XML text
         */
        struct Xml final
        {
            std::string xml; ///< The XML text
            friend bool operator==(const Xml& lhs, const Xml& rhs) { return lhs.xml == rhs.xml; }
            friend bool operator!=(const Xml& lhs, const Xml& rhs) { return !(lhs == rhs); }
        };

        /**
         * \brief Json text
         */
        struct Json final
        {
            std::string json; ///< The json text
            friend bool operator==(const Json& lhs, const Json& rhs) { return lhs.json == rhs.json; }
            friend bool operator!=(const Json& lhs, const Json& rhs) { return !(lhs == rhs); }
        };

        /**
         * \brief QQ mini programs
         */
        struct App final
        {
            std::string content; ///< The content
            friend bool operator==(const App& lhs, const App& rhs) { return lhs.content == rhs.content; }
            friend bool operator!=(const App& lhs, const App& rhs) { return !(lhs == rhs); }
        };

        /**
         * \brief QQ poke message
         */
        struct Poke final
        {
            std::string name; ///< Type of the poke message
            friend bool operator==(const Poke& lhs, const Poke& rhs) { return lhs.name == rhs.name; }
            friend bool operator!=(const Poke& lhs, const Poke& rhs) { return !(lhs == rhs); }
        };

        void to_json(utils::json& json, const Source& value);
        void from_json(const utils::json& json, Source& value);
        void to_json(utils::json& json, const Quote& value);
        void from_json(const utils::json& json, Quote& value);
        void to_json(utils::json& json, const At& value);
        void from_json(const utils::json& json, At& value);
        void to_json(utils::json& json, const AtAll& value);
        void from_json(const utils::json& json, AtAll& value);
        void to_json(utils::json& json, const Face& value);
        void from_json(const utils::json& json, Face& value);
        void to_json(utils::json& json, const Plain& value);
        void from_json(const utils::json& json, Plain& value);
        void to_json(utils::json& json, const Image& value);
        void from_json(const utils::json& json, Image& value);
        void to_json(utils::json& json, const FlashImage& value);
        void from_json(const utils::json& json, FlashImage& value);
        void to_json(utils::json& json, const Xml& value);
        void from_json(const utils::json& json, Xml& value);
        void to_json(utils::json& json, const Json& value);
        void from_json(const utils::json& json, Json& value);
        void to_json(utils::json& json, const App& value);
        void from_json(const utils::json& json, App& value);
        void to_json(utils::json& json, const Poke& value);
        void from_json(const utils::json& json, Poke& value);

        using Variant = std::variant<At, AtAll, Face, Plain, Image,
            FlashImage, Xml, Json, App, Poke>;
    }

    /**
     * \brief Enum corresponding to every type of a message chain node
     */
    enum class SegmentType
    {
        at, at_all, face, plain, image,
        flash_image, xml, json, app, poke
    };

    /**
     * \brief Type representing a single segment in the message chain
     */
    class Segment final :
        public utils::VariantWrapper<msg::Variant, SegmentType>
    {
    private:
        using Base = VariantWrapper<msg::Variant, SegmentType>;
    public: // Expose special members from the base class
        using Base::Base;
        using Base::operator=;
        friend bool operator==(const Segment& lhs, const Segment& rhs)
        {
            return static_cast<const Base&>(lhs) == static_cast<const Base&>(rhs);
        }
        friend bool operator!=(const Segment& lhs, const Segment& rhs) { return !(lhs == rhs); }
    };

    void to_json(utils::json& json, const Segment& value);
    void from_json(const utils::json& json, Segment& value);
    void to_json(utils::json& json, const Message& value);
}
