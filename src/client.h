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
    Client(const Client& client) = delete;
    Client(Client&& client) noexcept = delete;
    Client& operator=(const Client& rhs) = delete;
    Client& operator=(Client&& rhs) noexcept = delete;

public:
    //passive socket listening and updating
    void sendMessage(int requestID, const std::string& message);
    //createSession
    void createSession(int requestID);

private:
    std::jthread chatLogUpdateThread;
    boost::asio::io_context ioServiceContext;
    std::mutex activeSessionGuard;
    std::map<int, std::shared_ptr<Session>> sessions;
};



#endif //IRC_CLIENT_H
