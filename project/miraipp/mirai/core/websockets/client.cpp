#include "client.h"
#include "../common.h"
#include "../../utils/functional.h"

namespace mirai::ws
{
    Client::Client()
    {
        client_.clear_access_channels(wspp::log::alevel::all);
        client_.clear_error_channels(wspp::log::alevel::all);
        client_.init_asio();
        client_.start_perpetual();
        thread_ = utils::Thread([&client = client_]()
        {
            try { client.run(); }
            catch (...) { error_logger(); }
        });
    }

    Client::~Client() noexcept
    {
        client_.stop_perpetual();
        for (const auto& connection : connections_)
        {
            try
            {
                if (!connection->ended())
                    close(*connection);
            }
            catch (...) {}
        }
    }

    Connection& Client::connect(const std::string& uri)
    {
        std::error_code error;
        const auto ptr = client_.get_connection(uri, error);
        if (error) throw RuntimeError(error.message());
        auto& connection = *connections_.emplace_back(
            std::make_unique<Connection>(ptr->get_handle(), uri));
        ptr->set_open_handler(utils::bind_front(&Connection::on_open,
            std::ref(connection), std::ref(client_)));
        ptr->set_fail_handler(utils::bind_front(&Connection::on_fail,
            std::ref(connection), std::ref(client_)));
        ptr->set_close_handler(utils::bind_front(&Connection::on_close,
            std::ref(connection), std::ref(client_)));
        ptr->set_message_handler(utils::bind_front(&Connection::on_message,
            std::ref(connection)));
        client_.connect(ptr);
        return connection;
    }

    void Client::close(Connection& connection)
    {
        std::error_code error;
        client_.close(connection.handle(),
            wspp::close::status::going_away, {}, error);
        if (error) throw RuntimeError(error.message());
    }
}
