#include "TCPServer.h"

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

TcpServer::TcpServer(int port, CommandHandler handler)
    : port_(port), handler_(handler), is_running_(false), server_fd_(-1) {}

TcpServer::~TcpServer() {
    stop();
}

void TcpServer::start() {
    if (is_running_) return;

    is_running_ = true;
    server_thread_ = std::thread(&TcpServer::run, this);
}

void TcpServer::stop() {
    if (!is_running_) return;

    is_running_ = false;

    if (server_fd_ != -1) {
        shutdown(server_fd_, SHUT_RDWR);
        close(server_fd_);
        server_fd_ = -1;
    }

    if (server_thread_.joinable()) {
        server_thread_.join();
    }

    Log::info("Stopping TcpServer...");
}

void TcpServer::run() {
    struct sockaddr_in address{};
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        Log::error("Socket failed");
        return;
    }

    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        Log::error("setsockopt failed");
        return;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(server_fd_, (struct sockaddr *)&address, sizeof(address)) < 0) {
        Log::error("bind failed");
        return;
    }

    if (listen(server_fd_, 3) < 0) {
        Log::error("listen failed");
        return;
    }

    Log::info("TCPServer listening at port " + std::to_string(port_));

    while (is_running_) {
        int new_socket;
        if ((new_socket = accept(server_fd_, (struct sockaddr *)&address,
                                 (socklen_t*)&addrlen)) < 0) {
            if (!is_running_) break;
            Log::error("accept failed");
            continue;
        }

        std::thread(&TcpServer::handleClient, this, new_socket).detach();
    }
}

void TcpServer::handleClient(int client_socket) {
    char buffer[1024] = {0};
    ssize_t valread;

    while ((valread = read(client_socket, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[valread] = '\0';
        std::string command(buffer);

        // Remove espaços e quebras de linha no início e fim
        command.erase(0, command.find_first_not_of(" \r\n"));
        command.erase(command.find_last_not_of(" \r\n") + 1);

        Log::debug("Command received: " + command);

        // Obtem a resposta do handler
        std::string response = handler_(command);

        // Envia a resposta para o cliente
        if (!response.empty()) {
            ssize_t sent = send(client_socket, response.c_str(), response.size(), 0);
            Log::debug("Resposta enviada: " + response);
            if (sent == -1) {
                Log::error("Failed to send response");
                break; // Pode encerrar conexão se enviar falhar
            }
        }
    }

    shutdown(client_socket, SHUT_RDWR);
    close(client_socket);
}
