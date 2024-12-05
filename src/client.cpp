#include "client.h"

Client::Client(int numThreads) : workGuard(ioServiceContext.get_executor()){
    for (unsigned char i = 1; i <= numThreads; ++i) {
        std::unique_ptr<std::thread> thread{new std::thread{ [this](){ioServiceContext.run();} }};
        threads.push_back(std::move(thread));
    }
}

void Client::cancelRequest(int requestID) {
    //lock new sessions form being made
    std::unique_lock lock{activeSessionGuard};
    auto elemIter = sessions.find(requestID);
    if(elemIter != sessions.end()){
        std::unique_lock cancelSessionLock{elemIter->second->cancelGuard};
        elemIter->second->cancel = true;
        elemIter->second->socket.close();
    }
}

void Client::close() {
    //destroy work object
    workGuard.reset();

    //close thread for

    for(auto& thread : threads){
        thread->join();
    }

}

void Client::onRequestComplete(std::shared_ptr<Session> session) {
    //shut down the session
    session->socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);

    //delete session from map
    std::unique_lock lock{activeSessionGuard};
    auto elemIter = sessions.find(static_cast<int>(session->requestID));
    if(elemIter != sessions.end()){
        sessions.erase(elemIter);
    }

    //set error code
    boost::system::error_code errorCode;
    if (session->errorCode.value() == 0 && session->cancel) {
        errorCode = boost::asio::error::operation_aborted;
    } else {
        errorCode = session->errorCode;
    }

    //fire completion handler
    session->callback(session->requestID, session->response, session->errorCode);
}

void Client::sendMessage(int requestID, const std::string& message) {
    std::shared_ptr<Session> session;

    {
        std::lock_guard lock{activeSessionGuard};
        session = sessions[requestID];
        session->payload = message;
    }

    session->socket.wait(boost::asio::socket_base::wait_type::wait_write);
    std::lock_guard lock{session->sessionWrite};

    boost::system::error_code writeEC;
    boost::asio::write(session->socket, boost::asio::buffer(session->payload), writeEC);

    if(writeEC.value() != 0)
        spdlog::info("sock write error");
    else
        spdlog::info("written");

}

void Client::createSession(int requestID) {
    //create session
    auto newSession = std::make_shared<Session>(ioServiceContext, "192.168.50.226", 6667, "", requestID, handler);

    //keep the session alive
    boost::asio::socket_base::keep_alive option;
    //newSession->socket.set_option(option);

    //open socket ready to be used
    newSession->socket.open(newSession->endpoint.protocol());

    //open session to endpoint
    newSession->socket.connect(newSession->endpoint);

    chatLogUpdateThread = std::move(std::jthread([newSession](){
        std::string strBuffer;

        for(;;){
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
            auto bytesRead = boost::asio::read(newSession->socket, boost::asio::buffer(strBuffer));
            //spdlog::info("Read End");

            if(bytesRead != 0){
                spdlog::info("response: {}", strBuffer);
            } else {
            //    spdlog::info("no data");
            }

            strBuffer.clear();
        }

    }));

    //detach thread
    chatLogUpdateThread.detach();

    //lock sessions mutex
    //add new session to sessions map
    std::lock_guard lock{activeSessionGuard};
    sessions[requestID] = newSession;
}

