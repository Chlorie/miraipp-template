#include "connection.h"

namespace mirai::ws
{
    const char* to_string(const Status e)
    {
        switch (e)
        {
            case Status::connecting: return "connecting";
            case Status::open: return "open";
            case Status::failed: return "failed";
            case Status::closed: return "closed";
            default: return "unknown";
        }
    }

    // NOLINTNEXTLINE(performance-unnecessary-value-param)
    void Connection::on_open(AsioClient& client, const Handle handle)
    {
        status_ = Status::open;
        const auto ptr = client.get_con_from_hdl(handle);
        server_ = ptr->get_response_header("Server");
    }

    // NOLINTNEXTLINE(performance-unnecessary-value-param)
    void Connection::on_fail(AsioClient& client, const Handle handle)
    {
        status_ = Status::failed;
        const auto ptr = client.get_con_from_hdl(handle);
        server_ = ptr->get_response_header("Server");
        error_ = ptr->get_ec();
    }

    // NOLINTNEXTLINE(performance-unnecessary-value-param)
    void Connection::on_close(AsioClient& client, const Handle handle)
    {
        status_ = Status::closed;
        const auto ptr = client.get_con_from_hdl(handle);
        error_ = ptr->get_ec();
    }

    // NOLINTNEXTLINE(performance-unnecessary-value-param)
    void Connection::on_message(Handle, const AsioClient::message_ptr message) const
    {
        if (!message_callback_) return;
        message_callback_(message);
    }

    void Connection::message_callback(std::function<void(const AsioClient::message_ptr&)> callback)
    {
        message_callback_ = std::move(callback);
    }
}
