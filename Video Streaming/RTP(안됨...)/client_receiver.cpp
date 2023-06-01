// g++ -o client.out client.cpp

#include <iostream>
#include <fstream>
#include <cstdint>
#include <netinet/in.h>
#include <arpa/inet.h>
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

// 비디오 전송을 위한 RTP 스트리밍
void sendVideoWithRTP(const char* videoPath, const char* serverIP, int serverPort) {
    std::ifstream videoFile(videoPath, std::ios::binary);
    if (!videoFile) {
        std::cerr << "Failed to open video file" << std::endl;
        return;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(serverPort);

    char rtpHeader[RTP_HEADER_SIZE] = {0};
    rtpHeader[0] = 0x80;
    rtpHeader[1] = 0x60;

    uint16_t sequenceNumber = 0;
    uint32_t timestamp = 0;

    while (!videoFile.eof()) {
        char frameData[MAX_PAYLOAD_SIZE];
        videoFile.read(frameData, sizeof(frameData));
        size_t bytesRead = videoFile.gcount();

        if (bytesRead <= 0) {
            break;
        }

        rtpHeader[2] = (sequenceNumber >> 8) & 0xFF;
        rtpHeader[3] = sequenceNumber & 0xFF;
        rtpHeader[4] = (timestamp >> 24) & 0xFF;
        rtpHeader[5] = (timestamp >> 16) & 0xFF;
        rtpHeader[6] = (timestamp >> 8) & 0xFF;
        rtpHeader[7] = timestamp & 0xFF;

        sendto(sock, rtpHeader, RTP_HEADER_SIZE, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        sendto(sock, frameData, bytesRead, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

        sequenceNumber++;
        timestamp += 3600;
    }
    close(sock);
}

int main() {
    const char* videoPath = "video.mp4";
    const char* serverIP = "127.0.0.1";
    int serverPort = 5004;
    sendVideoWithRTP(videoPath, serverIP, serverPort);
    return 0;
}
