#ifndef IRC_CLIENT_H
#define IRC_CLIENT_H

#include "boost/asio.hpp"
#include "spdlog/spdlog.h"
#include "session.h"

#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <map>
//manages the start up and shutdown processes
//coordinates interation between components
//client initializes the ConnectionManager, UserInterface, StateManager, and EventLoop
//begins the main event loop

class Client {
public:
    explicit Client(int numThreads);
    ~Client() = default;

    //remove ability to copy/move
    Client(const Client& client) = delete;
    Client(Client&& client) noexcept = delete;
    Client& operator=(const Client& rhs) = delete;
    Client& operator=(Client&& rhs) noexcept = delete;

public:
    //passive socket listen for updates
    void cancelRequest(int requestID);
    void close();

    //passive socket listening and updating
    void listenOnPort(const std::string& ipAddress, unsigned short port, int requestID);

    void sendRequest(const std::string& ipAddress, unsigned short port, int requestID, std::string payload);

private:
    void onRequestComplete(std::shared_ptr<Session> session);

private:
    std::jthread chatLogUpdateThread;
    boost::asio::io_service ioServiceContext;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> workGuard;
    std::mutex activeSessionGuard;
    std::map<int, std::shared_ptr<Session>> sessions;
    std::vector<std::unique_ptr<std::thread>> threads;
};



#endif //IRC_CLIENT_H
