#ifndef IRC_SESSION_H
#define IRC_SESSION_H

#include <utility>

#include "boost/asio.hpp"
#include "boost/asio/error.hpp"
#include "spdlog/spdlog.h"

struct Session {
    Session(boost::asio::io_service& ioService, const std::string& rawIpAddress, unsigned short port, unsigned int id) :
            socket(ioService),
            endpoint(boost::asio::ip::address::from_string(rawIpAddress), port),
            requestID{id} {};

    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::endpoint endpoint;
    unsigned int requestID;
    std::mutex cancelGuard;
    std::mutex sessionWrite;
};


#endif //IRC_SESSION_H
