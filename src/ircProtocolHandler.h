#ifndef IRC_IRCPROTOCOLHANDLER_H
#define IRC_IRCPROTOCOLHANDLER_H

//Parses raw messages received from the server into structure formats
//Encodes user commands into protocol-compliant messages


class IRCProtocolHandler {
public:
    void parseMessage();
    void constructMessage();

private:

};


#endif //IRC_IRCPROTOCOLHANDLER_H
