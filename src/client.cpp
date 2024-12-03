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

void Client::listenOnPort(const std::string &ipAddress, unsigned short port, int requestID) {
    //create session
    std::shared_ptr<Session> session = std::make_shared<Session>(ioServiceContext, ipAddress, port, " ", requestID, handler);

    //listens for incoming connections
    spdlog::info("Server is listening on port: {}", port);
    boost::asio::ip::tcp::acceptor acceptor(ioServiceContext, session->endpoint);

    chatLogUpdateThread = std::move(std::jthread([session](boost::asio::ip::tcp::acceptor acceptor){

        std::string strBuffer;
        boost::asio::dynamic_string_buffer dynamicBuffer = boost::asio::dynamic_buffer(strBuffer);

        for(;;) {
            //check if session should be stopped

            //wait for the it to become ready to read
            spdlog::info("waiting");
            acceptor.wait(boost::asio::ip::tcp::acceptor::wait_type::wait_read);

            //async wait for a incoming connection
            acceptor.async_accept(session->socket, [session, &dynamicBuffer, &strBuffer](const boost::system::error_code &err) {
                spdlog::info("accepted");
                if (!err) {
                    //read data from socket
                    session->socket.read_some(dynamicBuffer.data(0, dynamicBuffer.size()));
                    //print response
                    spdlog::info("response: {}", strBuffer);
                    //clear buffer
                    dynamicBuffer.shrink(0);
                }
            });

        }

    }, std::move(acceptor)));
    //detach a thread
    chatLogUpdateThread.detach();

    //add session to map
    std::unique_lock<std::mutex> lock{activeSessionGuard};
    sessions[requestID] = session;
}

void Client::sendRequest(const std::string &ipAddress, unsigned short port, int requestID) {
    std::shared_ptr<Session> session = std::make_shared<Session>(ioServiceContext, ipAddress, port, " ", requestID, handler);

    session->socket.open(session->endpoint.protocol());

    session->socket.async_connect(session->endpoint, [this, session](const boost::system::error_code& onAsyncConnectEC){
        if (onAsyncConnectEC.value() != 0) {
            session->errorCode = onAsyncConnectEC;
            spdlog::info("connected");
            return;
        }

    });


    //lock session guard
    //add session to map
}
