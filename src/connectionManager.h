#ifndef IRC_CONNECTIONMANAGER_H
#define IRC_CONNECTIONMANAGER_H

//handles all network communications
//establish and maintains the connection to the iirc server
//reads from and writes to the socket asycn
//handles reconnection logic

class ConnectionManager {
public:
    void connect();
    void disconnect();
    void sendData();
    void receiveData();

private:
};


#endif //IRC_CONNECTIONMANAGER_H
