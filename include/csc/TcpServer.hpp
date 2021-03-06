/**
 * @file tcp_server.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-09-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#ifndef CSC_TCP_SERVER_HPP
#define CSC_TCP_SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <vector>
#include <string>

#include "csc/PacketFactory.hpp"

#define HIC clients[info.cliRef]

namespace csc {

    enum class ClientState : uint8_t {
        handshake, status = 1, login = 2, play
    };

    enum class HandshakePid : uint8_t { handshaking = 0x00, legacyServerPing = 0xFE };
    enum class ServBoundStatusPid : uint8_t { request = 0x00, ping = 0x01 };
    enum class CliBoundStatusPid : uint8_t { response = 0x00, pong = 0x01 };
    enum class ServBoundLoginPid : uint8_t { loginStart = 0x00, encryptionResponse = 0x01, pluginResponse = 0x02 };
    enum class CliBoundLoginPid : uint8_t { disconnect = 0x00, encryptionRequest = 0x01, loginSuccess = 0x02, setCompression = 0x03, pluginRequest = 0x04 };

    enum class ServBoundPlayPid : uint8_t {

    };

    enum class CliBoundPlayPid : uint8_t {
        joinGame = 0x26, 
    };

    struct HandlingInfo {
        int8_t *dat;
        int id;
        int len;
        int cliRef;
    };

    struct ClientData;

    struct ClientData {
        int sock;

        ClientState state = ClientState::handshake;
        bool compressed = false;

        inline VariantPacketFactory newPacketFactory(int id, unsigned size) {
            return compressed ? VariantPacketFactory{CompressedPacketFactory{id, size}} : VariantPacketFactory{PacketFactory{id, size}};
        };

    };

    class TcpServer {
    private:
        int sock;
        std::vector<ClientData> clients;

        void handleHandshake(HandlingInfo &info);
        void handleStatus(HandlingInfo &info);
        void handleLogin(HandlingInfo &info);

    public:
        TcpServer() = delete;
        TcpServer(const char *ip, const char *port);
        ~TcpServer();

        void tickAccept();
        void tickClient(int cliRef);

        inline int numClients() { return clients.size(); }

        void dropConnection(int cliRef);

        TcpServer(const TcpServer &rhs) = delete;
        TcpServer &operator=(const TcpServer &rhs) = delete;
    };
}

#endif