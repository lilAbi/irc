#include <iostream>
#include <string>
#include "client.h"
//#include "synchapi.h"

int main() {
    //create io context
    //Client ircClient("192.168.50.226", 6667);
    //ircClient.connect();
    //ircClient.close();
    //initialize irc class and sub components

    //set server ip

    //establish connection to public-irc.w3.org 6697
    //client ircClient{boost::asio::io, server, ip}
    //ircClient


    Client client{1};
    //client.listenOnPort("0.0.0.0", 6667, 1);
    //client.sendRequest("192.168.50.226", 6667, 2, "CAP LS 302");
    //client.sendRequest("127.0.0.1", 6667, 2, "");
    //client.sendRequest("192.168.50.226", 6667, 3, "NICK dan");
    //client.sendRequest("192.168.50.226", 6667, 4, "USER d * 0 :This is a really good name");

    client.createSession(1);
    client.sendMessage(1, "CAP LS 302\r\n");
    client.sendMessage(1, "NICK dan\r\n");
    client.sendMessage(1, "USER d * 0 :This is a really good name\r\n");


    sleep(120);

    //starts input loop
    /*
    while(true){
        std::string message;
        std::cout << "[~]> ";
        std::getline(std::cin, message);

        spdlog::info("{}", message);
        if (message == "exit") break;
    }
     */

    return 0;
}
