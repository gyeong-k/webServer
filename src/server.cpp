#include "../include/server.h"
#include "../include/requestHandler.h"
#include <sys/socket.h>
#include <netinet/in.h> //인터넷 주소관련 구조체와 상수  / sockaddr_in
#include <unistd.h>     //  유닉스 시스템 호출 API / 파일 입출력, 프로세스 관리, 소켓 관리를 포함한 여러 기능을 제공 close()
#include <cstring>      // memset() - 메모리 초기화
#include <thread>
#include <string>




WebServer :: WebServer(int port) : port(port) {};

//소켓 설정
void WebServer::start() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); //주소체계(IPv4), 소켓유형(TCP), 프로로톨(0:TCP) 설정
    if (server_fd == -1 ) throw std::runtime_error("소켓 생성 실패");

    //주소와 포트 저장
    sockaddr_in address{};      //아이피 주소와 포트 정보를 포함하는 구조체 (타입임)
    address.sin_family = AF_INET;   //주소체계
    address.sin_addr.s_addr = INADDR_ANY;   //ip주소 저장, INADDR_ANY-> 서버의 모든 ip 주소 사용 가능
    address.sin_port = htons(port);  //포트 번호 저장, htons() - 호스트 바이트 순서에서 네트워크 바이트 순서로 변환해 설정
                                            //서로 다른 아키텍처와 운영체제를 사용하는 컴퓨터 간에 데이터의 일관성을 유지
                                            //TCP/IP와 같은 네트워크 프로토콜은 기본적으로 빅 엔디안 방식을 사용
    //소켓에 주소와 포트 할당
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address))<0)        //소켓 변수, 서버 주소 구조체, 서버주소구조체 크기
        throw  std::runtime_error("포트 바인딩 실패");
    
    //소켓을 수신 상태로 전환 - 클라이언트의 연결 요청 대기
    if (listen(server_fd, 3))        //소켓, 대기열의 최대 길이 (이 갯수만큼 동시 요청 가능)
        throw std::runtime_error("서버 리스닝 실패");
    
    handdleConnections(server_fd);
}

//클라리언트 요청 수락 소켓 생성 및 스레드로 요청 처리
void WebServer::handdleConnections(int server_fd) {
    while(true) {   //서버는 항상 클라이언트의 요청을 받을 준비를 해야하므로 while 처리
        //accept() 함수는 연결 요청이 들어올 때까지 프로그램의 실행을 멈추고 대기
        //요청이 들어오지 않는 동안 서버는 계속해서 accept()를 호출하여 기다리
        int clinet_socket = accept(server_fd, nullptr, nullptr);    //클라이언트 연결수락, 생성된 클라이언트 소켓의 파일 디스크립터를 반환 => 서버는 client_socket을 통해 클라이언트와 통신할 수 있
        //양수 : 성공  /  음수 : (-1: 오류)
        if (clinet_socket < 0)  continue;   //오류 - 무시하고 다음 연결 요천 대기
        
        //스레드 이용해 각 요청 처리 - 실행할 메서드, 함수를 실행할 RequestHandler 객체(임시로 생성)
        //RequestHandler::processRequest는 RequestHandler 클래스에 속한 멤버 함수입니다. 멤버 함수는 객체 없이 독립적으로 호출할 수 없기 때문에, 함수 포인터로 사용하려면 멤버 함수의 주소를 명시적으로 지정
        //detach() : 메인 스레드는 스레드 종료를 기다리지 않고 계속 진행(스레드 분리)
         std::thread(&RequestHandler::processRequest, RequestHandler(clinet_socket)).detach();
        //join(): 스레드가 종료될 때까지 기다리는 함수입니다.
        //detach(): 스레드를 메인 스레드와 독립적으로 실행하도록 분리하는 함수입니다.
    }

}
