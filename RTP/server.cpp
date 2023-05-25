// g++ -o server server.cpp $(pkg-config --libs --cflags opencv4) -lpthread -lboost_system
#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::udp;

const int RTP_PORT = 5004;  // RTP 포트 번호

void handleReceive(const boost::system::error_code& error, size_t bytes_transferred,
                   udp::socket& socket, array<char, 8192>& buffer)
{
    if (!error || error == boost::asio::error::message_size)
    {
        // 수신한 RTP 패킷 처리
        const char* data = buffer.data();
        // TODO: RTP 패킷 처리 로직 추가
        
        // 다음 패킷을 받기 위해 비동기 수신 대기
        socket.async_receive(boost::asio::buffer(buffer),
                             [&](const boost::system::error_code& error, size_t bytes_transferred)
                             {
                                 handleReceive(error, bytes_transferred, socket, buffer);
                             });
    }
}

int main()
{
    try
    {
        boost::asio::io_service ioService;
        
        // RTP 소켓 생성 및 바인딩
        udp::endpoint localEndpoint(udp::v4(), RTP_PORT);
        udp::socket socket(ioService, localEndpoint);
        
        // 수신 버퍼 생성
        array<char, 8192> buffer;
        
        // 비동기 수신 대기
        socket.async_receive(boost::asio::buffer(buffer),
                             [&](const boost::system::error_code& error, size_t bytes_transferred)
                             {
                                 handleReceive(error, bytes_transferred, socket, buffer);
                             });
        
        // 이벤트 루프 실행
        ioService.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << endl;
    }
    
    return 0;
}
