#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <thread>
#include <functional>
#include <string>
#include <atomic>
#include <mutex>

class TcpServer {
public:
    using CommandHandler = std::function<void(const std::string&)>;

    TcpServer(int port, CommandHandler handler);
    ~TcpServer();

    void start();
    void stop();

private:
    int port_;
    int server_fd_;
    std::thread server_thread_;
    std::atomic<bool> is_running_;
    CommandHandler handler_;
    std::mutex socket_mutex_;

    void run();
    void handleClient(int client_socket);
};

#endif //_TCP_SERVER_H_