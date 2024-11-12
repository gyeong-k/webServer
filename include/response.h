#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>

class Response {
    public:
        void sendRes(int client_socket, const std::string& file_path, int status_code);
};

#endif