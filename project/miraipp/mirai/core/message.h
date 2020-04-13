#pragma once

#include <vector>
#include <string>
#include "../utils/json_extensions.h"
#include "../utils/variant_wrapper.h"

namespace mirai
{
    /**
     * \brief Type representing a single node in the message chain
     */
    class MessageChainNode;

    // TODO: separate received message (with source & quote) and sent message (without those)
    // TODO: wrap the vector into a convenient class for message processing, since dealing with variants are crappy

    /**
     * \brief A chain of message chain nodes, being the whole message
     */
    using MessageChain = std::vector<MessageChainNode>;

    namespace msg // TODO: needs documentation
    {
        /**
         * \brief The source of a message
         */
        struct Source final
        {
            int32_t id = 0; ///< The message ID of this message
            int32_t time = 0; ///< The timestamp when this message was sent
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
        };

        /**
         * \brief Mentioning someone
         */
        struct At final
        {
            int32_t target = 0; ///< Mentioned group member ID
            std::string display; ///< The string for display the @ message
        };

        /**
         * \brief Mentioning everyone
         */
        struct AtAll final {};

        /**
         * \brief QQ emoji
         */
        struct Face final
        {
            std::optional<int32_t> face_id = 0; ///< The ID of the emoji
            std::optional<std::string> name; ///< The name of the emoji
        };

        /**
         * \brief Plain text message segment
         */
        struct Plain final
        {
            std::string text;
        };

        /**
         * \brief An image
         */
        struct Image final
        {
            std::optional<std::string> image_id; ///< The ID of the image
            std::optional<std::string> url; ///< The URL of the image
            std::optional<std::string> path; ///< The relative path to "plugins/MiraiAPIHTTP/images" of a local image
        };

        /**
         * \brief A flash image
         */
        struct FlashImage final
        {
            std::optional<std::string> image_id; ///< The ID of the image
            std::optional<std::string> url; ///< The URL of the image
            std::optional<std::string> path; ///< The relative path to "plugins/MiraiAPIHTTP/images" of a local image
        };

        /**
         * \brief XML text
         */
        struct Xml final
        {
            std::string xml; ///< The XML text
        };

        /**
         * \brief Json text
         */
        struct Json final
        {
            std::string json; ///< The json text
        };

        /**
         * \brief QQ mini programs
         */
        struct App final
        {
            std::string content; ///< The content
        };

        /**
         * \brief QQ poke message
         */
        struct Poke final
        {
            std::string name; ///< Type of the poke message
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

        using Variant = std::variant<Source, Quote, At, AtAll, Face, Plain,
            Image, FlashImage, Xml, Json, App, Poke>;
    }

    /**
     * \brief Enum corresponding to every type of a message chain node
     */
    enum class MessageChainNodeType
    {
        source, quote, at, at_all, face, plain,
        image, flash_image, xml, json, app, poke
    };

    class MessageChainNode final : // Forward declaring type alias is impossible, using inheritance here
        public utils::VariantWrapper<msg::Variant, MessageChainNodeType>
    {
    private:
        using Base = VariantWrapper<msg::Variant, MessageChainNodeType>;
    public: // Expose special members from the base class
        using Base::Base;
        using Base::operator=;
    };

    /**
     * \brief Construct a plain text message object
     * \param text The message content
     * \return The message
     */
    MessageChain plain_text(std::string_view text);

    void to_json(utils::json& json, const MessageChainNode& value);
    void from_json(const utils::json& json, MessageChainNode& value);
}
