#pragma once

#include <vector>
#include <string>
#include "../utils/json_extensions.h"
#include "../utils/variant_wrapper.h"

namespace mirai
{
    class Segment;

    // TODO: separate received message (with source & quote) and sent message (without those)

    /**
     * \brief A collection of segments, being the whole message chain
     */
    using MessageChain = std::vector<Segment>;

    class StringifiedMessage;

    /**
     * \brief A message type wrapping message chain, suitable for modifying operations
     * \remarks This class is designed for constructing new messages more easily (for
     * sending new messages), but not designed for pattern matching of the message
     * (for parsing incoming messages). For parsing incoming messages, please use the
     * message wrapper type StringifiedMessage.
     */
    class Message final
    {
    private:
        MessageChain chain_;
    public:
        /**
         * \brief Create an empty message
         */
        Message() = default;

        /**
         * \brief Create a message with a message chain
         * \param chain The message chain
         */
        explicit Message(MessageChain chain);

        /**
         * \brief Construct a message with a single segment
         * \param node The message chain node
         */
        explicit Message(Segment node);

        /**
         * \brief Construct a message with the chain contained in a stringified message
         * \param message The stringified message
         */
        explicit Message(StringifiedMessage message); // TODO: redundant copy of the string

        /**
         * \brief Construct a message using a plain text string
         * \param plain_text The plain text string
         */
        explicit Message(std::string_view plain_text);

        /**
         * \brief Assign a message chain to this object
         * \param chain The message chain
         * \return Reference to this object
         */
        Message& operator=(MessageChain chain);

        /**
         * \brief Assign a message with only one segment to this object
         * \param node The message chain node
         * \return Reference to this object
         */
        Message& operator=(Segment node);

        /**
         * \brief Assign a message with the chain contained in a stringified message
         * \param message The stringified message
         * \return Reference to this object
         */
        Message& operator=(StringifiedMessage message);

        /**
         * \brief Assign a plain text string to this object
         * \param plain_text The plain text strin
         * \return Reference to this object
         */
        Message& operator=(std::string_view plain_text);

        /**
         * \brief Get the underlying message chain
         * \return The message chain
         */
        MessageChain& chain() { return chain_; }

        /**
         * \brief Get the underlying message chain
         * \return The message chain
         */
        const MessageChain& chain() const { return chain_; }

        /**
         * \brief The length of the message chain
         * \return The size
         */
        size_t size() const { return chain_.size(); }

        /**
         * \brief Check whether the message is empty
         * \return The result
         */
        bool empty() const { return chain_.empty(); }

        /**
         * \brief Append a segment to the end of this message
         * \param node The segment
         */
        void push_back(Segment node); // TODO: too many moves in these function family

        /**
         * \brief Append a segment to the end of this message
         * \param node The segment
         * \return Reference to this message
         */
        Message& emplace_back(Segment node);

        /**
         * \brief Append another message to this message
         * \param message The message to append
         * \return Reference to this message
         */
        Message& operator+=(const Message& message);

        /**
         * \brief Append another message to this message
         * \param message The message to append
         * \return Reference to this message
         */
        Message& operator+=(Message&& message);

        /**
         * \brief Append a message chain to this message
         * \param chain The message chain to append
         * \return Reference to this message
         */
        Message& operator+=(const MessageChain& chain) { return operator+=(Message(chain)); }

        /**
         * \brief Append a message chain to this message
         * \param chain The message chain to append
         * \return Reference to this message
         */
        Message& operator+=(MessageChain&& chain) { return operator+=(Message(std::move(chain))); }

        /**
         * \brief Append a segment to the end of this message
         * \param node The segment
         * \return Reference to this message
         */
        Message& operator+=(Segment node);

        /**
         * \brief Append a string of plain text to this message
         * \param plain_text The plain text to append
         * \return Reference to this message
         */
        Message& operator+=(std::string_view plain_text);

        /**
         * \brief Concatenate two messages together
         * \param lhs The first message
         * \param rhs The second message
         * \return The concatenated message
         */
        friend Message operator+(Message lhs, const Message& rhs) { return lhs += rhs; }

        /**
         * \brief Concatenate two messages together
         * \param lhs The first message
         * \param rhs The second message
         * \return The concatenated message
         */
        friend Message operator+(Message lhs, Message&& rhs) { return lhs += std::move(rhs); }

        /**
         * \brief Concatenate a message and a message chain together
         * \param lhs The message
         * \param rhs The message chain
         * \return The concatenated message
         */
        friend Message operator+(Message lhs, const MessageChain& rhs) { return lhs += rhs; }

        /**
         * \brief Concatenate a message and a message chain together
         * \param lhs The message
         * \param rhs The message chain
         * \return The concatenated message
         */
        friend Message operator+(Message lhs, MessageChain&& rhs) { return lhs += std::move(rhs); }

        /**
         * \brief Concatenate a message and a segment together
         * \param lhs The message
         * \param rhs The segment
         * \return The concatenated message
         */
        friend Message operator+(Message lhs, Segment rhs);

        /**
         * \brief Concatenate a message and a plain text string together
         * \param lhs The message
         * \param rhs The plain text string
         * \return The concatenated message
         */
        friend Message operator+(Message lhs, const std::string_view rhs) { return lhs += rhs; }

        /**
         * \brief Check if two messages are the same
         * \param lhs The first message
         * \param rhs The second message
         * \return The result
         * \remarks For more advanced checking use StringifiedMessage
         */
        friend bool operator==(const Message& lhs, const Message& rhs) { return lhs.chain_ == rhs.chain_; }

        /**
         * \brief Check if two messages are not the same
         * \param lhs The first message
         * \param rhs The second message
         * \return The result
         * \remarks For more advanced checking use StringifiedMessage
         */
        friend bool operator!=(const Message& lhs, const Message& rhs) { return !(lhs == rhs); }
    };

    // TODO: add more string matching utilities for this class
    // TODO: maybe introduce optional dependency for boost::regex since std::regex is too slow
    /**
     * \brief A message type wrapping message chain, suitable for pattern matching
     * \remarks This class is designed for pattern matching on the message, for parsing
     * incoming messages, and is not designed for building new messages, as you may see
     * from the lack of non-const member functions in this class. If you want easier
     * mutation and construction of message, please use the Message class.
     */
    class StringifiedMessage final
    {
    private:
        std::string string_;
        MessageChain chain_;

        void stringify(); // Initialize the string
    public:
        /**
         * \brief Create an empty message
         */
        StringifiedMessage() = default;

        /**
         * \brief Create a message with a message chain
         * \param chain The message chain
         */
        explicit StringifiedMessage(MessageChain chain);

        /**
         * \brief Assign a message chain to this object
         * \param chain The message chain
         * \return Reference to this object
         */
        StringifiedMessage& operator=(MessageChain chain);

        /**
         * \brief Get the underlying message chain
         * \return The message chain
         * \remarks StringifiedMessage class is meant for mutating the chain,
         * so no non-const lvalue reference version of this function is provided.
         * For mutating and constructing new messages, consider constructing a
         * Message object from this object, or use the non-const rvalue reference
         * version of this function for moving the message chain out.
         */
        const MessageChain& chain() const & { return chain_; }

        /**
         * \brief Move the underlying message chain out, leaving this object empty
         * \return The message chain
         */
        MessageChain chain() &&;

        /**
         * \brief The length of the message chain
         * \return The size
         */
        size_t size() const { return chain_.size(); }

        /**
         * \brief Check whether the message is empty
         * \return The result
         */
        bool empty() const { return chain_.empty(); }

        /**
         * \brief Get the concatenation of all plain text segments 
         * \return The concatenated text
         * \remarks When the text strings extracted from two messages compare equal,
         * it doesn't mean that the two messages must be the same. Use operator== for
         * exact comparison.
         */
        std::string extract_text() const;

        /**
         * \brief Get the stringified version of the message
         * \return The string
         * \remarks When two strings compare equal, it doesn't mean that the two messages
         * are the same. Use operator== for exact comparison.
         */
        const std::string& string() const { return string_; }

        /**
         * \brief Check if two messages are the same
         * \param lhs The first message
         * \param rhs The second message
         * \return The result
         * \remarks For more advanced checking use StringifiedMessage
         */
        friend bool operator==(const StringifiedMessage& lhs, const StringifiedMessage& rhs)
        {
            return lhs.chain_ == rhs.chain_;
        }

        /**
         * \brief Check if two messages are not the same
         * \param lhs The first message
         * \param rhs The second message
         * \return The result
         * \remarks For more advanced checking use StringifiedMessage
         */
        friend bool operator!=(const StringifiedMessage& lhs, const StringifiedMessage& rhs)
        {
            return !(lhs == rhs);
        }
    };

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
     * \brief Represents a message received from others (group or friend)
     */
    struct ReceivedMessage final
    {
        msg::Source source; ///< Source of the message
        std::optional<msg::Quote> quote; ///< If exists, quotation of the message
        StringifiedMessage content; ///< The real message content
    };

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
    void from_json(const utils::json& json, ReceivedMessage& value);
}
