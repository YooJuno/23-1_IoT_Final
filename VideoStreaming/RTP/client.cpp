// g++ -o client client.cpp $(pkg-config --libs --cflags opencv4) -lpthread -lboost_system
#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::udp;

const int RTP_PORT = 5004;  // RTP 포트 번호
const string SERVER_ADDRESS = "127.0.0.1";  // 서버 IP 주소

int main()
{
    try
    {
        boost::asio::io_service ioService;
        
        // RTP 소켓 생성 및 연결
        udp::endpoint serverEndpoint(boost::asio::ip::address::from_string(SERVER_ADDRESS), RTP_PORT);
        udp::socket socket(ioService);
        socket.open(udp::v4());
        

        // RTP 패킷 생성
        const std::string payload = "Sample RTP payload";
        boost::system::error_code ignoredError;
        socket.send_to(boost::asio::buffer(payload), serverEndpoint, 0, ignoredError);
        
        // 이벤트 루프 실행
        ioService.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << endl;
    }
    
    return 0;
}
