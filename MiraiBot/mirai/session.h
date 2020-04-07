#pragma once

#include <string>
#include "message.h"
#include "optional_param.h"

namespace mirai
{
    class Session final
    {
    private:
        int64_t qq_ = 0;
        std::string key_;
        void release() const noexcept;
    public:
        /**
         * \brief Construct a default invalid Session object
         */
        Session() = default;

        /**
         * \brief Start a new mirai-http session
         * \param auth_key The authorization key
         * \param qq The QQ ID for the bot
         */
        Session(std::string_view auth_key, int64_t qq);

        ~Session() noexcept;
        Session(const Session&) = delete;
        Session(Session&& other) noexcept;
        Session& operator=(const Session&) = delete;
        Session& operator=(Session&& other) noexcept;

        /**
         * \brief Check whether the session object is valid <br>
         * Default constructed or moved-from session objects are invalid
         * \return The result
         */
        bool is_valid() const { return qq_ != 0; }

        /**
         * \brief Get the session key
         * \return The key
         */
        std::string_view key() const { return key_; }

        /**
         * \brief Send message to a friend
         * \param target Target QQ to send the message to
         * \param msg The message to send
         * \param quote The message to be quoted (optional)
         * \return The message ID of the message sent
         */
        int32_t send_friend_message(int64_t target, const Message& msg,
            OptionalParam<int32_t> quote = {}) const;

        /**
         * \brief Send plain text message to a friend
         * \param target Target QQ to send the message to
         * \param msg The message to send
         * \param quote The message to be quoted (optional)
         * \return The message ID of the message sent
         */
        int32_t send_friend_message(int64_t target, std::string_view msg,
            OptionalParam<int32_t> quote = {}) const;

        /**
         * \brief Send message to a group
         * \param target Target group to send the message to
         * \param msg The message to send
         * \param quote The message to be quoted (optional)
         * \return The message ID of the message sent
         */
        int32_t send_group_message(int64_t target, const Message& msg, 
            OptionalParam<int32_t> quote = {}) const;

        /**
         * \brief Send plain text message to a group
         * \param target Target group to send the message to
         * \param msg The message to send
         * \param quote The message to be quoted (optional)
         * \return The message ID of the message sent
         */
        int32_t send_group_message(int64_t target, std::string_view msg, 
            OptionalParam<int32_t> quote = {}) const;
    };
}
