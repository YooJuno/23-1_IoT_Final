// g++ -o server.out server.cpp `pkg-config --cflags --libs opencv4`

#include <iostream>
#include <cstdint>
#include <opencv2/opencv.hpp>
#include <netinet/in.h>
#include <unistd.h>

#define RTP_HEADER_SIZE 12
#define MAX_PAYLOAD_SIZE 1400

// RTP 헤더 구조체 정의
typedef struct {
    uint8_t version:2;
    uint8_t padding:1;
    uint8_t extension:1;
    uint8_t csrcCount:4;
    uint8_t marker:1;
    uint8_t payloadType:7;
    uint16_t sequenceNumber;
    uint32_t timestamp;
    uint32_t ssrc;
} RTPHeader;

// RTP 스트리밍 수신 및 디코딩
void receiveAndDecodeRTP(int listenPort) {
    // 소켓 생성
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    // 서버 주소 설정
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(listenPort);

    // 소켓 바인딩
    if (bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        return;
    }

    uint8_t payloadType = 96;
    uint16_t expectedSequenceNumber = 0;
    cv::VideoWriter videoWriter;

    while (true) {
        char rtpHeader[RTP_HEADER_SIZE];
        sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);

        ssize_t bytesRead = recvfrom(sock, rtpHeader, RTP_HEADER_SIZE, 0, (struct sockaddr*)&clientAddr, &addrLen);
        if (bytesRead < 0) {
            std::cerr << "Failed to receive RTP header" << std::endl;
            continue;
        }

        uint8_t receivedPayloadType = rtpHeader[1];

        if (receivedPayloadType == payloadType) {
            uint16_t receivedSequenceNumber = (rtpHeader[2] << 8) | rtpHeader[3];
            uint32_t timestamp = (rtpHeader[4] << 24) | (rtpHeader[5] << 16) | (rtpHeader[6] << 8) | rtpHeader[7];

            if (receivedSequenceNumber != expectedSequenceNumber) {
                std::cerr << "Packet loss detected" << std::endl;
            }

            char frameData[MAX_PAYLOAD_SIZE];
            bytesRead = recv(sock, frameData, sizeof(frameData), 0);
            if (bytesRead < 0) {
                std::cerr << "Failed to receive frame data" << std::endl;
                continue;
            }

            cv::Mat frame(cv::Size(bytesRead, 1), CV_8UC1, frameData);

            if (!videoWriter.isOpened()) {
                videoWriter.open("output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, frame.size(), true);
            }

            videoWriter.write(frame);
            expectedSequenceNumber = receivedSequenceNumber + 1;
        }
    }
    videoWriter.release();
    close(sock);
}

int main() {
    int listenPort = 5004;
    receiveAndDecodeRTP(listenPort);
    return 0;
}
