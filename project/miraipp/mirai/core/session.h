#pragma once

#include <string>
#include "types.h"
#include "events.h"
#include "common.h"
#include "message/segment.h"
#include "websockets/client.h"
#include "../utils/optional_param.h"
#include "../utils/array_proxy.h"
#include "../utils/functional.h"
#include "../utils/request.h"
#include "../utils/string.h"

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
        std::unique_ptr<ws::Client> client_;
        std::unique_ptr<asio::thread_pool> thread_pool_;

        std::vector<std::string> send_image_message(utils::OptionalParam<int64_t> qq,
            utils::OptionalParam<int64_t> group,
            utils::ArrayProxy<std::string> urls) const;

        std::vector<Event> get_events(std::string_view url, size_t count) const;

        template <typename F, typename E>
        ws::Connection& subscribe(std::string_view url,
            F&& callback, E&& error_handler, ExecutionPolicy policy);
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
         * \brief Query whether the thread pool is started
         * \return The result
         */
        bool thread_pool_started() const { return thread_pool_ != nullptr; }

        /**
         * \brief Start the thread pool for future event processing
         * \param thread_count Thread count, leave as default for the default value
         * \remarks This function does nothing if the thread pool is already running
         */
        void start_thread_pool(utils::OptionalParam<size_t> thread_count = {});

        /**
         * \brief Join all threads in the thread pool and destroy the pool
         */
        void destroy_thread_pool();

        /**
         * \brief Query whether the websocket client is started
         * \return The result
         */
        bool websocket_client_started() const { return client_ != nullptr; }

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
         * \brief Send message to a temporary group member chat
         * \param qq Target QQ to send the message to
         * \param group Target group to start the temporary chat
         * \param msg The message to send
         * \param quote The message to be quoted (optional)
         * \return The message ID of the message sent
         */
        int32_t send_temp_message(int64_t qq, int64_t group, const Message& msg,
            utils::OptionalParam<int32_t> quote = {}) const;

        /**
         * \brief Send plain text message to a temporary group member chat
         * \param qq Target QQ to send the message to
         * \param group Target group to start the temporary chat
         * \param msg The message to send
         * \param quote The message to be quoted (optional)
         * \return The message ID of the message sent
         */
        int32_t send_temp_message(int64_t qq, int64_t group, std::string_view msg,
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
         * \brief Send images to a temporary chat via URLs
         * \param qq Target QQ
         * \param group Target group
         * \param urls Image URLs
         * \return The ID list of the images
         * \remark This function is supposed to be used only if you
         * need to retrieve image ID from URLs
         */
        std::vector<std::string> send_temp_image_message(int64_t qq, int64_t group,
            utils::ArrayProxy<std::string> urls) const;

        /**
         * \brief Upload an image to the server
         * \param type Target type (friend, group or temp)
         * \param path The path to the image file
         * \return The image ID and other info about the image
         * \remark Image IDs of the same image are different for
         * friend messages from group messages
         */
        msg::Image upload_image(TargetType type, const std::string& path) const;

        /**
         * \brief Recall a message
         * \param message_id The ID of the message to recall
         * \remarks As for now (mirai core 0.35), recalling friend messages are not supported
         */
        void recall(int32_t message_id) const;

        /**
         * \brief Pop oldest events from the event queue
         * \param count Amount of events to get
         * \return The events
         */
        std::vector<Event> fetch_events(size_t count = 10) const;

        /**
         * \brief Pop latest events from the event queue
         * \param count Amount of events to get
         * \return The events
         */
        std::vector<Event> fetch_latest_events(size_t count = 10) const;

        /**
         * \brief Peek oldest events from the event queue
         * \param count Amount of events to get
         * \return The events
         */
        std::vector<Event> peek_events(size_t count = 10) const;

        /**
         * \brief Peek latest events from the event queue
         * \param count Amount of events to get
         * \return The events
         */
        std::vector<Event> peek_latest_events(size_t count = 10) const;

        /**
         * \brief Count remaining events in the event queue
         * \return The count
         */
        size_t count_events() const;

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
         * \brief Respond to a new friend request event
         * \param event The event to respond to
         * \param type Type of the response
         * \param message The additional message
         */
        void respond_new_friend_request(const NewFriendRequestEvent& event,
            NewFriendResponseType type, std::string_view message) const;

        /**
         * \brief Respond to a member join request event
         * \param event The event to respond to
         * \param type Type of the response
         * \param message The additional message
         */
        void respond_member_join_request(const MemberJoinRequestEvent& event,
            MemberJoinResponseType type, std::string_view message) const;

        /**
         * \brief Set a group's config
         * \param target The group ID
         * \param config The config to set
         */
        void group_config(int64_t target, const GroupConfig& config) const;

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
         * \param name The new name for the member
         * \param special_title The new special title for the member
         */
        void member_info(int64_t group, int64_t member,
            utils::OptionalParam<std::string_view> name,
            utils::OptionalParam<std::string_view> special_title) const;

        /**
         * \brief Get a group member's info
         * \param group The group ID
         * \param member The member ID
         * \return The result
         */
        MemberInfo member_info(int64_t group, int64_t member) const;

        /**
         * \brief Close the websocket client, outstanding connections will
         * also be closed
         */
        void close_websocket_client();

        /**
         * \brief Close a WebSocket connection
         * \param connection The connection
         */
        void close_connection(ws::Connection& connection);

        /**
         * \brief Listen on message events received using the callback
         * \tparam F Type of the callback
         * \tparam E Type of the error handler
         * \param callback The callback
         * \param error_handler The error handler
         * \param policy Execution policy of this connection
         * \returns The Websocket connection
         * \remarks The callback should be able to visit variants with
         * both of the message types. The events will be handled on
         * another thread, so if any exception is not handled the
         * application will abort.
         */
        template <typename F, typename E,
            typename = std::invoke_result_t<F, Event&>,
            typename = std::invoke_result_t<E>>
        ws::Connection& subscribe_messages(F&& callback, E&& error_handler,
            ExecutionPolicy policy = ExecutionPolicy::single_thread);

        /**
         * \brief Listen on non-message events received using the callback
         * \tparam F Type of the callback
         * \tparam E Type of the error handler
         * \param callback The callback
         * \param error_handler The error handler
         * \param policy Execution policy of this connection
         * \returns The Websocket connection
         * \remarks The callback should be able to visit variants with
         * all of the non-message event types. The events will be
         * handled on another thread, so if any exception is not handled
         * the application will abort.
         */
        template <typename F, typename E,
            typename = std::invoke_result_t<F, Event&>,
            typename = std::invoke_result_t<E>>
        ws::Connection& subscribe_non_message(F&& callback, E&& error_handler,
            ExecutionPolicy policy = ExecutionPolicy::single_thread);

        /**
         * \brief Listen on all events received using the callback
         * \tparam F Type of the callback
         * \tparam E Type of the error handler
         * \param callback The callback
         * \param error_handler The error handler
         * \param policy Execution policy of this connection
         * \returns The Websocket connection
         * \remarks The callback should be able to visit variants with
         * all of the event types. The events will be handled on another
         * thread, so if any exception is not handled the application
         * will abort.
         */
        template <typename F, typename E,
            typename = std::invoke_result_t<F, Event&>,
            typename = std::invoke_result_t<E>>
        ws::Connection& subscribe_all_events(F&& callback, E&& error_handler,
            ExecutionPolicy policy = ExecutionPolicy::single_thread);

        /**
         * \brief Set the config of this session, leave parameters as default for
         * not changing that setting
         * \param cache_size The new cache size
         * \param enable_websocket Whether to enable websocket
         */
        void config(utils::OptionalParam<size_t> cache_size = {},
            utils::OptionalParam<bool> enable_websocket = {}) const;

        /**
         * \brief Get the config of this session
         * \return The config
         */
        SessionConfig config() const;
    };

    template <typename F, typename E>
    ws::Connection& Session::subscribe(const std::string_view url,
        F&& callback, E&& error_handler, const ExecutionPolicy policy)
    {
        using MsgPtr = ws::AsioClient::message_ptr;
        if (!client_) client_ = std::make_unique<ws::Client>();
        const std::string uri = utils::strcat("ws://", base_url, url, "?sessionKey=", key_);
        ws::Connection& con = client_->connect(uri);
        if (policy == ExecutionPolicy::single_thread)
        {
            con.message_callback([callback = std::forward<F>(callback),
                    error_handler = std::forward<E>(error_handler)](const MsgPtr& msg)
                {
                    try
                    {
                        const utils::json json = utils::json::parse(msg->get_payload());
                        utils::check_response(json);
                        Event e = json.get<Event>();
                        callback(e);
                    }
                    catch (...) { error_handler(); }
                });
        }
        else
        {
            if (!thread_pool_) start_thread_pool();
            // Wrap everything into shared_ptrs to avoid lifetime issues
            con.message_callback([
                    &pool = *thread_pool_,
                    callback = std::make_shared<std::decay_t<F>>(std::forward<F>(callback)),
                    error_handler = std::make_shared<std::decay_t<E>>(std::forward<E>(error_handler))
                ](const MsgPtr& msg)
                {
                    try
                    {
                        asio::post(pool, [=]() // Copy the shared_ptrs to make them alive
                        {
                            try
                            {
                                const utils::json json = utils::json::parse(msg->get_payload());
                                utils::check_response(json);
                                Event e = json.get<Event>();
                                (*callback)(e);
                            }
                            catch (...) { (*error_handler)(); }
                        });
                    }
                    catch (...) { (*error_handler)(); }
                });
        }
        return con;
    }

    template <typename F, typename E, typename, typename>
    ws::Connection& Session::subscribe_messages(F&& callback, E&& error_handler,
        const ExecutionPolicy policy)
    {
        return subscribe("/message",
            std::forward<F>(callback), std::forward<E>(error_handler), policy);
    }

    template <typename F, typename E, typename, typename>
    ws::Connection& Session::subscribe_non_message(F&& callback, E&& error_handler,
        const ExecutionPolicy policy)
    {
        return subscribe("/event",
            std::forward<F>(callback), std::forward<E>(error_handler), policy);
    }

    template <typename F, typename E, typename, typename>
    ws::Connection& Session::subscribe_all_events(F&& callback, E&& error_handler,
        const ExecutionPolicy policy)
    {
        return subscribe("/all",
            std::forward<F>(callback), std::forward<E>(error_handler), policy);
    }
}
