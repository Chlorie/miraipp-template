#pragma once

#include <websocketpp/connection.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <functional>

namespace mirai::ws
{
    namespace wspp = websocketpp;

    /**
     * \brief Status of a WebSocket connection
     */
    enum class Status { connecting, open, failed, closed };

    /**
     * \brief Get a string representation of the status
     * \param e The status
     * \return The C-string
     */
    const char* to_string(Status e);

    using AsioClient = wspp::client<wspp::config::asio_client>;

    /**
     * \brief A WebSocket connection
     * \remarks Connections are only supposed to be constructed on the heap
     */
    class Connection final
    {
    private:
        using Handle = wspp::connection_hdl;
        Handle handle_;
        Status status_ = Status::connecting;
        std::error_code error_{};
        std::string uri_;
        std::string server_ = "N/A";
        std::function<void(const AsioClient::message_ptr&)> message_callback_{};
    public:
        /**
         * \brief Construct a connection object using a handle and an URI
         * \param handle The connection handle
         * \param uri The URI
         */
        Connection(Handle handle, const std::string_view uri) :
            handle_(std::move(handle)), uri_(uri) {}

        /**
         * \brief Destroy a connection object
         * \remarks Closing is done by the client, not the connection object
         */
        ~Connection() noexcept = default;

        /**
         * \brief Connections cannot be copied
         */
        Connection(const Connection&) = delete;

        /**
         * \brief Connections cannot be moved
         */
        Connection(Connection&&) = delete;

        /**
         * \brief Connections cannot be copied
         */
        Connection& operator=(const Connection&) = delete;

        /**
         * \brief Connections cannot be moved
         */
        Connection& operator=(Connection&&) = delete;

        /**
         * \brief This function is called when the connection opens
         * \param client The client
         * \param handle The connection handle
         */
        void on_open(AsioClient& client, Handle handle);

        /**
         * \brief This function is called when the connection fails to connect
         * \param client The client
         * \param handle The connection handle
         */
        void on_fail(AsioClient& client, Handle handle);

        /**
         * \brief This function is called when the connection is closed
         * \param client The client
         * \param handle The connection handle
         */
        void on_close(AsioClient& client, Handle handle);

        /**
         * \brief This function is called when the connection receives a message
         * \param message The message
         */
        void on_message(Handle, AsioClient::message_ptr message) const;

        /**
         * \brief Get the connection handle assiociated to this connection
         * \return The handle
         */
        Handle handle() const { return handle_; }

        /**
         * \brief Get the current status of this connection
         * \return The status
         */
        Status status() const { return status_; }

        /**
         * \brief Check whether the connection is ended
         * \return The result
         */
        bool ended() const { return status_ == Status::failed || status_ == Status::closed; }

        /**
         * \brief Get the error code of the connection if an error occurred
         * \return The error code
         */
        std::error_code error() const { return error_; }

        /**
         * \brief Get the error reason of the connection if an error occurred
         * \return The error reason
         */
        std::string error_reason() const { return error_.message(); }

        /**
         * \brief Get the URI this connection is connected to
         * \return The URI
         */
        const std::string& uri() const { return uri_; }

        /**
         * \brief Get a string describing the server
         * \return The string
         */
        const std::string& server() const { return server_; }

        /**
         * \brief Set the message callback of this connection
         * \param callback The callback to set
         */
        void message_callback(std::function<void(const AsioClient::message_ptr&)> callback);

        /**
         * \brief Get the message callback of this connection
         * \return The callback
         */
        const auto& message_callback() const { return message_callback_; }
    };
}
