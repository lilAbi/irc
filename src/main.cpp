#include <iostream>
#include <string>
#include "client.h"


//simple irc client
//to connect


int main() {

    //ask user for ip

    //ask user for username



    Client client{1};

    client.createSession(1);

    sleep(20);

    client.sendMessage(1, "CAP LS 302\r\n");
    client.sendMessage(1, "NICK dan\r\n");
    client.sendMessage(1, "USER myusername 0 * :Jeff Green\r\n");

    sleep(5);
    client.sendMessage(1, "MOTD\r\n");

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
