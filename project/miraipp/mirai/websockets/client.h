#pragma once

#include <unordered_map>
#include "connection.h"
#include "../utils/thread.h"

namespace mirai::ws
{
    namespace wspp = websocketpp;

    class Client final // TODO: needs documentation
    {
    private:
        AsioClient client_;
        utils::Thread thread_;
        std::vector<std::unique_ptr<Connection>> connections_;

    public:
        Client();
        ~Client() noexcept;
        Client(const Client&) = delete;
        Client(Client&&) noexcept = default;
        Client& operator=(const Client&) = delete;
        Client& operator=(Client&&) = delete;

        Connection& connect(const std::string& uri);
        void close(Connection& connection);

        const auto& connections() const { return connections_; }
    };
}
