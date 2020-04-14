#pragma once

#include <string_view>
#include <vector>

namespace mirai
{
    class Segment;

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
}
