#include "client.h"

Client::Client(int numThreads) {
}

void Client::sendMessage(int requestID, const std::string& message) {
    std::shared_ptr<Session> session;

    {
        std::lock_guard lock{activeSessionGuard};
        session = sessions[requestID];
    }

    session->socket.wait(boost::asio::socket_base::wait_type::wait_write);
    std::lock_guard lock{session->sessionWrite};
    spdlog::info("write lock grabbed");

    boost::system::error_code writeEC;
    boost::asio::write(session->socket, boost::asio::buffer(message), writeEC);

    if(writeEC.value() != 0)
        spdlog::info("sock write error");
}

void Client::createSession(int requestID) {
    //create session
    auto newSession = std::make_shared<Session>(ioServiceContext, "192.168.50.226", 6667, "", requestID, handler);

    //keep the session alive

    //open socket ready to be used
    newSession->socket.open(newSession->endpoint.protocol());
    boost::asio::socket_base::keep_alive option;
    newSession->socket.set_option(option);

    //open session to endpoint
    newSession->socket.connect(newSession->endpoint);

    chatLogUpdateThread = std::move(std::jthread([newSession](){

        std::string stringBuffer;
        for(;;){
            boost::asio::dynamic_string_buffer dynamicStringBuffer{stringBuffer};
            //wait for the socket to become readable
            //spdlog::info("Read Waiting Starting");
            newSession->socket.wait(boost::asio::socket_base::wait_type::wait_read);
            //spdlog::info("Read Waiting Ended");

            //lock session socketLock from being writeable
            //spdlog::info("Read Lock Starting");
            std::lock_guard readLock{newSession->sessionWrite};
            //spdlog::info("Read Lock Ended");

            //read from the socket
            //spdlog::info("Read Start");
            //auto bytesRead = boost::asio::read(newSession->socket, boost::asio::buffer(strBuffer));
            auto bytesRead = boost::asio::read_until(newSession->socket, dynamicStringBuffer, '\n');
            //spdlog::info("Read End");

            if(bytesRead != 0){
                spdlog::info("{}", stringBuffer);
            } else {
            //    spdlog::info("no data");
            }
            stringBuffer.clear();
        }

    }));

    //detach thread
    chatLogUpdateThread.detach();

    //lock sessions mutex
    //add new session to sessions map
    std::lock_guard lock{activeSessionGuard};
    sessions[requestID] = newSession;
}

