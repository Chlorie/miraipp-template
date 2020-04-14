#pragma once

#include <unordered_map>
#include "connection.h"
#include "../../utils/thread.h"

namespace mirai::ws
{
    namespace wspp = websocketpp;

    /**
     * \brief WebSocket client
     */
    class Client final
    {
    private:
        utils::Thread thread_;
        AsioClient client_;
        std::vector<std::unique_ptr<Connection>> connections_;
    public:
        /**
         * \brief Start a WebSocket client on another thread
         */
        Client();

        /**
         * \brief Close all outstanding connections opened by this client,
         * close this client, and join the thread
         */
        ~Client() noexcept;

        /**
         * \brief Clients cannot be copied
         */
        Client(const Client&) = delete;

        /**
         * \brief Construct a client from a temporary client object
         */
        Client(Client&&) noexcept = default;

        /**
         * \brief Clients cannot be copied
         */
        Client& operator=(const Client&) = delete;

        /**
         * \brief Clients cannot be move assigned to
         * \remarks The underlying ASIO client forbids move assignment
         */
        Client& operator=(Client&&) = delete;

        /**
         * \brief Connection to a specific URI
         * \param uri The URI
         * \return A reference to the connection for closing the connection later
         */
        Connection& connect(const std::string& uri);

        /**
         * \brief Close a connection opened by this client
         * \param connection The connection to close
         */
        void close(Connection& connection);

        /**
         * \brief Get all the connections started by this client
         * \return The connections
         */
        const auto& connections() const { return connections_; }
    };
}
