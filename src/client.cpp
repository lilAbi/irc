

#include "client.h"

Client::Client(const std::string& ipAddress, unsigned short port)
:   endpoint{boost::asio::ip::address::from_string(ipAddress), port},
    socket{ioServiceContext} {
    //open socket
    socket.open(endpoint.protocol());
}

void Client::close() {
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    socket.close();
}

void Client::connect() {
    //initiate
    socket.connect(endpoint);
    std::string severMessage = receiveResponse();
    spdlog::info("Server Message: {}", severMessage);
}

std::string Client::receiveResponse() {
    boost::asio::streambuf buffer;
    std::string response;

    boost::asio::read_until(socket, buffer, '\n');

    std::istream inputStream(&buffer);
    std::getline(inputStream, response);

    return response;
}

void Client::sendRequest(const std::string request) {
    boost::asio::write(socket, boost::asio::buffer(request));
}