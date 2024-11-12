#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H
#include <string>

class RequestHandler {
    public:
        RequestHandler(int client_socket);
        void processRequest();

    private:
        int client_socket;
        std::string receiveRequest();
};


#endif 