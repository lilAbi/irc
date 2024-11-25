#ifndef IRC_CLIENT_H
#define IRC_CLIENT_H
#include "boost/asio.hpp"
#include "spdlog/spdlog.h"
//manages the start up and shutdown processes
//coordinates interation between components
//client initializes the ConnectionManager, UserInterface, StateManager, and EventLoop
//begins the main event loop

class Client {
public:
    //set endpoint
    Client(const std::string& ipAddress, unsigned short port);
    void close();
    void connect();

private:
    std::string receiveResponse();
    void sendRequest(const std::string request);

private:
    boost::asio::io_service ioServiceContext;
    boost::asio::ip::tcp::endpoint endpoint;
    boost::asio::ip::tcp::socket socket;
};


#endif //IRC_CLIENT_H
