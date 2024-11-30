
#include "session.h"

void handler(unsigned int requestID, const std::string& response, const boost::system::error_code& ec){
    if(ec.value() == 0){
        spdlog::info("Request {}, has completed. Response: \n{}", requestID, response);
    } else if (ec == boost::asio::error::operation_aborted) {
        spdlog::info("Request {}, has been canceled by the user.", requestID);
    } else {
        spdlog::info("Request #{} failed! Error code = {}. Error message: {}", requestID, ec.value(), ec.message());
    }
}

