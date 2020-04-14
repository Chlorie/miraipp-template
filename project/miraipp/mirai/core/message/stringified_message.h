#pragma once

#include "message.h"

namespace mirai
{
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
}
