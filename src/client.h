#ifndef IRC_CLIENT_H
#define IRC_CLIENT_H


//manages the start up and shutdown processes
//coordinates interation between components
//client initializes the ConnectionManager, UserInterface, StateManager, and EventLoop
//begins the main event loop

class Client {
public:
    Client(std::string ip, int port);

    bool initialize();
    void shutdown();

private:

};


#endif //IRC_CLIENT_H
