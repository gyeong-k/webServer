#include "../include/server.h"
#include <iostream>

int main(){
    try{
        WebServer server(8080); //  포트 넘겨
        server.start();
    }catch(const std::exception& e) {
        std::cerr << "서버 시작 실패: " << e.what() << std::endl;
    }
    return 0;
}

