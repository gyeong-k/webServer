#include "../include/response.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>


void Response::sendRes(int client_socket, const std::string& file_path, int status_code){
    std::ifstream file(file_path);   //파일에서 데이터 읽어 오기
    std::ostringstream response;    //문자열조립 -> str()로 최종 전체 문자열 얻음

    if(!file) {
        response << "HTTP/1.1 500 Internal Server Error\n\n";
    } else {
        response << "HTTP/1.1 " << status_code << " OK\n";
        response << "Content-Type: text/html\n\n";
        response << file.rdbuf();   //파일 내용을 본문에 추가
    }
    //str().c_str() : string으로 변환 후 C 스타일 문자열(const char*)로 변환(send함수에서 필요로 함)
    send(client_socket, response.str().c_str(), response.str().size(), 0);
}
