#include "../include/requestHandler.h"
#include "../include/response.h"
#include <unistd.h>
#include <iostream>
#include <string>

RequestHandler::RequestHandler(int client_socket) : client_socket(client_socket){}

void RequestHandler::processRequest() {
    std::string request = receiveRequest();
    Response response;
    
    //npos : 의 문자열 관련 라이브러리에서 사용되는 상수로, 찾는 문자열이 없음을 나타내는 값 : 문자열 탐색에서 실패
    if (request.find("GET /hello") != std::string::npos) {
        response.sendRes(client_socket, "../templates/hello.html", 200);
    } else if (request.find("GET /404") != std::string::npos) {
        response.sendRes(client_socket, "../templates/404.html", 200);
    } else {
        response.sendRes(client_socket, "../templates/index.html", 404);
    }
    close(client_socket);
}

std::string RequestHandler::receiveRequest() {
    char buffer[1024] = {0};
    //클라이언트 소켓에서 데이터 읽음 -> 읽은 데이터 버퍼에 저장
    read(client_socket, buffer, sizeof(buffer)-1);
    return std::string(buffer);
}
