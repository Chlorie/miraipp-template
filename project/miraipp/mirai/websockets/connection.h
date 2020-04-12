#pragma once

#include <websocketpp/connection.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <functional>

namespace mirai::ws // TODO: needs documentation
{
    namespace wspp = websocketpp;

    enum class Status { connecting, open, failed, closed };
    const char* to_string(Status e);

    using AsioClient = wspp::client<wspp::config::asio_client>;

    class Connection final
    {
    private:
        using Handle = wspp::connection_hdl;
        Handle handle_;
        Status status_ = Status::connecting;
        std::error_code error_{};
        std::string uri_;
        std::string server_ = "N/A";
        std::function<void(const std::string&)> message_callback_{};
    public:
        Connection(Handle handle, const std::string_view uri) :
            handle_(std::move(handle)), uri_(uri) {}
        ~Connection() noexcept = default;
        Connection(const Connection&) = delete;
        Connection(Connection&&) = delete;
        Connection& operator=(const Connection&) = delete;
        Connection& operator=(Connection&&) = delete;

        void on_open(AsioClient& client, Handle handle);
        void on_fail(AsioClient& client, Handle handle);
        void on_close(AsioClient& client, Handle handle);
        void on_message(Handle, AsioClient::message_ptr message) const;

        Handle handle() const { return handle_; }
        Status status() const { return status_; }
        bool ended() const { return status_ == Status::failed || status_ == Status::closed; }
        std::error_code error() const { return error_; }
        std::string error_reason() const { return error_.message(); }
        const std::string& uri() const { return uri_; }
        const std::string& server() const { return server_; }

        void message_callback(std::function<void(const std::string&)> callback);
        const auto& message_callback() const { return message_callback_; }
    };
}
