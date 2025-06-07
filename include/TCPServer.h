#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <string>
#include <functional>
#include <thread>
#include <atomic>

#include "Commons.h"

using CommandHandler = std::function<void(const std::string&)>;

class TcpServer {
public:
    TcpServer(int port, CommandHandler handler);
    ~TcpServer();

    void start();
    void stop();

private:
    void run();
    void handleClient(int client_socket);

    int port_;
    CommandHandler handler_;
    std::atomic<bool> is_running_;
    int server_fd_;
    std::thread server_thread_;
};

#endif // TCPSERVER_H
