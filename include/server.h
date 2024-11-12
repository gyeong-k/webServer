#ifndef SERVER_H
#define SERVER_H

class WebServer {
    public:
        WebServer(int port);
        void start();
    private:
        int port;
        void handdleConnections(int server_fd);
};

#endif