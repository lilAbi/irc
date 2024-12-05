#ifndef IRC_SESSION_H
#define IRC_SESSION_H

#include <utility>

#include "boost/asio.hpp"
#include "boost/asio/error.hpp"
#include "spdlog/spdlog.h"

typedef void (*RequestCallbackFunc)(unsigned int request_id, const std::string& response, const boost::system::error_code& ec);

void handler(unsigned int requestID, const std::string& response, const boost::system::error_code& ec);

struct Session {
    Session(boost::asio::io_service& ioService, const std::string& rawIpAddress, unsigned short port,
            std::string req, unsigned int id, RequestCallbackFunc func) :
            socket(ioService),
            endpoint(boost::asio::ip::address::from_string(rawIpAddress), port),
            payload(std::move(req)),
            requestID{id},
            callback{func} {};

    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::endpoint endpoint;
    std::string payload;    //string to hold request to send over socket

    boost::asio::streambuf responseBuffer;
    std::string response;

    boost::system::error_code errorCode;

    unsigned int requestID;
    RequestCallbackFunc callback;
    bool cancel{false};
    std::mutex cancelGuard;
    std::mutex sessionWrite;
};


#endif //IRC_SESSION_H
