/* Diagnostic Client library
* Copyright (C) 2023  Avijit Dey
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#pragma once

// includes
#include <boost/asio.hpp>
#include <vector>
#include "tcp_types.h"
#include "client/client_tcp_iface.h"
#include <boost/asio/ssl.hpp>

namespace boost_support {
namespace socket {
namespace tcp {
    using ssl_config = tsp_client::ssl_config;
    // type alias for tcp socket
    using Tcp = boost::asio::ip::tcp;
    using TcpSocket = Tcp::socket;

    class CreateTcpServerSocket {
    public:
        // Tcp function template used for reception
        using TcpHandlerRead = std::function<void(TcpMessagePtr)>;

        // Tcp Server connection class to create connection with client
        class TcpServerConnection {
        public:
            // ctor
            TcpServerConnection(boost::asio::io_context &io_context, TcpHandlerRead &&tcp_handler_read);

            // ctor
            TcpServerConnection(boost::asio::io_context &io_context,
                                boost::asio::ssl::context &ssl_context,
                                TcpHandlerRead &&tcp_handler_read);

            // dtor
            ~TcpServerConnection() = default;

            // move ctor
            TcpServerConnection(TcpServerConnection &&) = default;

            // move assignment
            TcpServerConnection &operator=(TcpServerConnection &&) = default;

            // copy ctor & assignment deleted
            TcpServerConnection(TcpServerConnection &) = delete;

            TcpServerConnection &operator=(TcpServerConnection &) = delete;

            // Get reference to underlying socket
            TcpSocket &get_socket();

            // Get reference to underlying ssl socket
            boost::asio::ssl::stream<TcpSocket> &get_ssl_socket();

            // function to transmit tcp message
            bool transmit(TcpMessageConstPtr udp_tx_message);

            // function to handle read
            bool received_message();

            // function to close the socket
            bool shutdown();

        private:
            // tcp socket
            std::unique_ptr<TcpSocket> tcp_socket_;
            // handler read
            TcpHandlerRead tcp_handler_read_;
            // ssl tcp socket
            std::unique_ptr<boost::asio::ssl::stream<TcpSocket>> tcp_socket_ssl_;
        };

    public:
        // type alias for tcp accepter
        using TcpAcceptor = boost::asio::ip::tcp::acceptor;

        // ctor
        CreateTcpServerSocket(std::string local_ip_address, uint16_t local_port_num, const ssl_config& ssl_cfg);

        // dtor
        ~CreateTcpServerSocket() = default;

        // Blocking function get a tcp connection
        TcpServerConnection get_tcp_server_connection(TcpHandlerRead &&tcp_handler_read);

    private:
        // local Ip address
        std::string local_ip_address_;
        // local port number
        uint16_t local_port_num_;
        // tcp socket accepter
        std::unique_ptr<TcpAcceptor> tcp_acceptor_{};
        // boost io context
        boost::asio::io_context io_context_;
        boost::asio::ssl::context ssl_context_{boost::asio::ssl::context::tlsv12};
        const ssl_config& ssl_cfg_;
        // Support tls
        bool support_tls_{false};
    };

}  // namespace tcp
}  // namespace socket
}  // namespace boost_support