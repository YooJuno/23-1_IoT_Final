// g++ -o server server.cpp $(pkg-config --libs --cflags opencv4)
#include<iostream>

// TCP header
#include "opencv4/opencv2/opencv.hpp"
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h> 
#include <string.h>
#include <pthread.h>


using namespace std;
using namespace cv;



int main(int argc, char **argv){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cout << "Failed to create socket." << endl;
        return 1;
    }
    
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));
    
    if (bind(sockfd, (sockaddr*)&server, sizeof(server)) == -1) {
        cout << "Failed to bind socket." << endl;
        return 1;
    }
    
    if (listen(sockfd, 10) == -1) {
        cout << "Failed to listen on socket." << endl;
        return 1;
    }
    
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    int clientfd = accept(sockfd, (sockaddr*)&client, &clientSize);
    if (clientfd == -1) {
        cout << "Failed to accept connection." << endl;
        return 1;
    }


    while(1){
        // 이미지 수신
        uint32_t size = 0;
        if (recv(clientfd, &size, sizeof(size), MSG_WAITALL) != sizeof(size)) {
            cout << "Failed to receive size." << endl;
            break;
        }
        
        vector<char> buffer(size);
        if (recv(clientfd, buffer.data(), size, MSG_WAITALL) != size) {
            cout << "Failed to receive data." << endl;
            break;
        }
        // while (send(clientfd, buffer.data(), size, 0) == -1) {
        //     cout << "Failed to send data." << endl;
        //     break;
        // }
        //send(clientfd, buffer.data(), size)

        
        Mat frame = imdecode(buffer, IMREAD_COLOR);
        if (frame.empty()) {
            cout << "Failed to decode image." << endl;
            break;
        }
        
        imshow("IoT", frame);
        waitKey(1);
    }



    return 0;
}