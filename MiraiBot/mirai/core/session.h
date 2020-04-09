#pragma once

#include <string>
#include "message.h"
#include "types.h"
#include "events.h"
#include "../utils/optional_param.h"
#include "../utils/array_proxy.h"

namespace mirai
{
    /**
     * \brief Type representing a session in the HTTP API
     * \details The session is released automatically in the destructor, thus
     * you don't need to manually free the resources. Session objects cannot be
     * copied, but can be moved to transfer ownership.
     */
    class Session final
    {
    private:
        int64_t qq_ = 0;
        std::string key_;
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

        /**
         * \brief Free the resource of a session
         */
        ~Session() noexcept;

        /**
         * \brief Sessions cannot be duplicated
         */
        Session(const Session&) = delete;

        /**
         * \brief Take the ownership of a session from another object
         * \param other The other object
         */
        Session(Session&& other) noexcept;

        /**
         * \brief Sessions cannot be duplicated
         */
        Session& operator=(const Session&) = delete;

        /**
         * \brief Take the ownership of a session from another object
         * \param other The other object
         * \return Reference to this object
         */
        Session& operator=(Session&& other) noexcept;

        /**
         * \brief Swap this session object with another
         * \param other The other object
         */
        void swap(Session& other) noexcept;

        /**
         * \brief Swap two session objects
         * \param lhs The first object
         * \param rhs The second object
         */
        friend void swap(Session& lhs, Session& rhs) noexcept { lhs.swap(rhs); }

        /**
         * \brief Check whether the session object is valid <br>
         * Default constructed or moved-from session objects are invalid
         * \return The result
         */
        bool is_valid() const { return qq_ != 0; }

        /**
         * \brief Get QQ of the bot associated with this session
         * \return The QQ number
         */
        int64_t qq() const { return qq_; }

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
            utils::OptionalParam<int32_t> quote = {}) const;

        /**
         * \brief Send plain text message to a friend
         * \param target Target QQ to send the message to
         * \param msg The message to send
         * \param quote The message to be quoted (optional)
         * \return The message ID of the message sent
         */
        int32_t send_friend_message(int64_t target, std::string_view msg,
            utils::OptionalParam<int32_t> quote = {}) const;

        /**
         * \brief Send message to a group
         * \param target Target group to send the message to
         * \param msg The message to send
         * \param quote The message to be quoted (optional)
         * \return The message ID of the message sent
         */
        int32_t send_group_message(int64_t target, const Message& msg,
            utils::OptionalParam<int32_t> quote = {}) const;

        /**
         * \brief Send plain text message to a group
         * \param target Target group to send the message to
         * \param msg The message to send
         * \param quote The message to be quoted (optional)
         * \return The message ID of the message sent
         */
        int32_t send_group_message(int64_t target, std::string_view msg,
            utils::OptionalParam<int32_t> quote = {}) const;

        /**
         * \brief Send images via URLs
         * \param target Target ID to send the message to
         * \param type Target type (friend or group)
         * \param urls Image URLs
         * \return The ID list of the images
         * \remark This function is supposed to be used only if you
         * need to retrieve image ID from URLs
         */
        std::vector<std::string> send_image_message(int64_t target, TargetType type,
            utils::ArrayProxy<std::string> urls) const;

        /**
         * \brief Send images to a friend via URLs
         * \param target Target QQ to send the message to
         * \param urls Image URLs
         * \return The ID list of the images
         * \remark This function is supposed to be used only if you
         * need to retrieve image ID from URLs
         */
        std::vector<std::string> send_friend_image_message(int64_t target,
            utils::ArrayProxy<std::string> urls) const;

        /**
         * \brief Send images to a group via URLs
         * \param target Target group to send the message to
         * \param urls Image URLs
         * \return The ID list of the images
         * \remark This function is supposed to be used only if you
         * need to retrieve image ID from URLs
         */
        std::vector<std::string> send_group_image_message(int64_t target,
            utils::ArrayProxy<std::string> urls) const;

        /**
         * \brief Upload an image to the server
         * \param type Target type (friend or group)
         * \param path The path to the image file
         * \return The image ID and other info about the image
         * \remark Image IDs of the same image are different for
         * friend messages from group messages
         */
        msg::Image upload_image(TargetType type, const std::string& path) const;

        /**
         * \brief Recall a message
         * \param message_id The ID of the message to recall
         */
        void recall(int32_t message_id) const; // TODO: return whether the operation is success

        /**
         * \brief Fetch an amount of events
         * \param count Amount of events to fetch
         * \return The events
         * \remark This function is called fetchMessage in the HTTP API,
         * but here the events and messages altogether are called "events"
         */
        std::vector<Event> fetch_events(size_t count = 10) const;

        /**
         * \brief Get the content of a message from its ID
         * \param id The ID of the message
         * \return The message object
         */
        Event message_from_id(int32_t id) const;

        /**
         * \brief Get the friend list of the bot
         * \return A vector of friends
         */
        std::vector<Friend> friend_list() const;

        /**
         * \brief Get the group list of the bot
         * \return A vector of groups
         */
        std::vector<Group> group_list() const;

        /**
         * \brief Get the member list of a group
         * \param target The group ID
         * \return A vector of members
         */
        std::vector<Member> member_list(int64_t target) const;

        /**
         * \brief Mute a group
         * \param target The group ID
         */
        void mute_all(int64_t target) const;

        /**
         * \brief Unmute a group
         * \param target The group ID
         */
        void unmute_all(int64_t target) const;

        /**
         * \brief Mute a group member
         * \param group The group ID
         * \param member The member ID
         * \param time_in_seconds Mute duration in seconds
         */
        void mute(int64_t group, int64_t member, int32_t time_in_seconds = 0) const;

        /**
         * \brief Unmute a group member
         * \param group The group ID
         * \param member The member ID
         */
        void unmute(int64_t group, int64_t member) const;

        /**
         * \brief Kick a group member
         * \param group The group ID
         * \param member The member ID
         * \param message The remark message for kicking the member
         */
        void kick(int64_t group, int64_t member, std::string_view message = "") const;

        /**
         * \brief Set a group's config
         * \param target The group ID
         * \param config The config to set
         */
        void group_config(int64_t target, const GroupConfig& config) const; // TODO: fields should be optional

        /**
         * \brief Get a group's config
         * \param target The group ID
         * \return The result
         */
        GroupConfig group_config(int64_t target) const;

        /**
         * \brief Modify a group member's info
         * \param group The group ID
         * \param member The member ID
         * \param info The member info to modify to
         */
        void member_info(int64_t group, int64_t member, const MemberInfo& info) const;

        /**
         * \brief Get a group member's info
         * \param group The group ID
         * \param member The member ID
         * \return The result
         */
        MemberInfo member_info(int64_t group, int64_t member) const;

        /**
         * \brief Set the config of this session
         * \param config The config to set to
         */
        void config(const SessionConfig& config) const;

        /**
         * \brief Get the config of this session
         * \return The config
         */
        SessionConfig config() const;
    };
}
