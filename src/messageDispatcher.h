#ifndef IRC_MESSAGEDISPATCHER_H
#define IRC_MESSAGEDISPATCHER_H

//Routes messages to the appropriate handlers based on message types
//Determines the action to take for each incoming message
//Invokes callbacks or handlers in other components
//  Updates StateManager if necessary.
//  Notifies UserInterface to display messages.

class MessageDispatcher {
public:
    void dispatchMessage();

private:

};


#endif //IRC_MESSAGEDISPATCHER_H
