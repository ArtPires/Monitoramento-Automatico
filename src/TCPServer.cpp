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
    is_running_ = true;
    server_thread_ = std::thread(&TcpServer::run, this);
}

void TcpServer::stop() {
    is_running_ = false;
    if (server_thread_.joinable()) {
        server_thread_.join();
    }
    if (server_fd_ > 0) {
        close(server_fd_);
    }
}

void TcpServer::run() {
    struct sockaddr_in address{};
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        return;
    }

    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        return;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(server_fd_, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return;
    }

    if (listen(server_fd_, 3) < 0) {
        perror("listen");
        return;
    }

    std::cout << "TCP Command Server ouvindo na porta " << port_ << std::endl;

    while (is_running_) {
        int new_socket;
        if ((new_socket = accept(server_fd_, (struct sockaddr *)&address,
                                 (socklen_t*)&addrlen)) < 0) {
            if (!is_running_) break;
            perror("accept");
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
        
        // Remove \r\n e espaços extras
        command.erase(command.find_last_not_of(" \r\n")+1);

        std::cout << "Comando recebido via TCP: " << command << std::endl;

        handler_(command);
    }

    close(client_socket);
}