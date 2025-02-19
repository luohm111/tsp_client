/**
* @file client.h
* @brief implementation of the tcp_client_iface based on boost_support.
* @author		wuting.xu
* @date		    2023/10/23
* @par Copyright(c):    2023 megatronix. All rights reserved.
*/

#pragma once

#include "include/client/client_tcp_iface.h"
#include "boost_support/socket/tcp/tcp_client.h"

namespace tsp_client {
    using TcpSocket = boost_support::socket::tcp::CreateTcpClientSocket;
    using TcpMessage = boost_support::socket::tcp::TcpMessageType;
    using TcpMessagePtr = boost_support::socket::tcp::TcpMessagePtr;

    class client_tcp_socket : public client_tcp_iface {
    public:
        //! ctor
        client_tcp_socket(const ssl_config& ssl_cfg):client_tcp_iface(ssl_cfg){};

        //! dtor
        ~client_tcp_socket(void) = default;

    public:
        //!
        //! start the tcp client
        //!
        //! \param addr host to be connected to
        //! \param port port to be connected to
        //!
        bool connect(const std::string &addr, std::uint32_t port);

        //!
        //! stop the tcp client
        //!
        //! \param wait_for_removal when sets to true, disconnect blocks until the underlying TCP client has been effectively removed from the io_service and that all the underlying callbacks have completed.
        //!
        void disconnect();

        //!
        //! \return whether the client is currently connected or not
        //!
        bool is_connected(void) const;

        //! Send tcp request
        //! \return whether the client is currently sent or not
        //!
        bool send(std::vector<uint8_t> &&request);

        //!
        //! set on disconnection handler
        //!
        //! \param disconnection_handler handler to be called in case of a disconnection
        //!
        void set_on_disconnection_handler(const disconnection_handler_t &disconnection_handler);
    private:
        //!
        //! tcp client for tsp
        //!
        std::unique_ptr<TcpSocket> tcp_socket_{nullptr};
        //!
        //! disconnection handler whenever a disconnection occurred
        //!
        disconnection_handler_t disconnection_handler_{nullptr};
        //!
        //! tcp client connected or not
        //!
        bool  connected_{false};
    };

} // namespace network
