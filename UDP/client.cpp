#include <iostream>
#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using boost::asio::ip::udp;

const string SERVER_ADDRESS = "127.0.0.1";  // 서버 IP 주소
const int SERVER_PORT = 5004;  // 서버 포트 번호
const int WIDTH = 640;  // 영상 가로 크기
const int HEIGHT = 480;  // 영상 세로 크기
const int FPS = 30;  // 영상 프레임 속도

void handleSend(const boost::system::error_code& error, size_t bytes_transferred,
                udp::socket& socket, const Mat& frame)
{
    if (!error)
    {
        // 다음 프레임을 전송하기 위해 비동기 전송 대기
        socket.async_send_to(boost::asio::buffer(frame.data, frame.total() * frame.elemSize()),
                             boost::bind(&handleSend, _1, _2, boost::ref(socket), boost::cref(frame)));
    }
    else
    {
        cerr << "Failed to send frame: " << error.message() << endl;
    }
}

int main()
{
    try
    {
        // 카메라 초기화
        VideoCapture cap(0);
        if (!cap.isOpened())
        {
            cerr << "Failed to open camera" << endl;
            return 1;
        }
        
        // 영상 속성 설정
        cap.set(CAP_PROP_FRAME_WIDTH, WIDTH);
        cap.set(CAP_PROP_FRAME_HEIGHT, HEIGHT);
        cap.set(CAP_PROP_FPS, FPS);
        
        // RTP 소켓 생성 및 연결
        boost::asio::io_service ioService;
        udp::endpoint serverEndpoint(boost::asio::ip::address::from_string(SERVER_ADDRESS), SERVER_PORT);
        udp::socket socket(ioService);
        socket.open(udp::v4());
        
        // 영상 전송 루프
        Mat frame;
        while (true)
        {
            // 영상 프레임 캡처
            cap.read(frame);
            
            // 영상 전송
            socket.send_to(boost::asio::buffer(frame.data, frame.total() * frame.elemSize()), serverEndpoint);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << endl;
    }
    
    return 0;
}
