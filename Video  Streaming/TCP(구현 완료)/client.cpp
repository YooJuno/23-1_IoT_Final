#include<iostream>
#include "opencv4/opencv2/opencv.hpp"
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <unistd.h> 
#include <string.h>
#include <vector>

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
    server.sin_addr.s_addr = inet_addr(argv[1]); 
    server.sin_port = htons(atoi(argv[2])); 
    
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        cout << "Failed to connect to server." << endl;
        return 1;
    }

    // Open the video file
    VideoCapture cap(-1); 
    if (!cap.isOpened()) {
        cout << "Failed to open video file." << endl;
        return 1;
    }

    while (1) {
        Mat frame;
        cap >> frame;

        // If the frame is empty, the video is probably finished
        if (frame.empty())
            continue;

        vector<uchar> buffer;
        imencode(".jpeg", frame, buffer);
        uint32_t size = buffer.size();

        if (send(sockfd, (char *)&size, sizeof(size), 0) < 0) {
            cout << "Failed to send size." << endl;
            break;
        }

        if (send(sockfd, buffer.data(), size, 0) < 0) {
            cout << "Failed to send frame." << endl;
            break;
        }
        // imshow("IoT", frame);

        cout<<"send\n";
        waitKey(33);
    }

    // Close the socket
    close(sockfd);

    return 0;
}
