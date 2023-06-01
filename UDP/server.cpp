#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;
using namespace cv;

int main() {
    const char* UDP_IP = "127.0.0.1";
    const int UDP_PORT = 5004;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_PORT);
    inet_aton(UDP_IP, &(server_addr.sin_addr));

    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        cerr << "Failed to bind socket." << endl;
        return -1;
    }

    vector<uchar> buffer[20];
    for (int i = 0; i < 20; i++) {
        buffer[i].resize(46080);
    }
    
    while (true) {
        int len = sizeof(server_addr);
        uchar data[46081];
        int n = recvfrom(sock, data, 46081, 0, (struct sockaddr*)&server_addr, (socklen_t*)&len);
        if (n != 46081) {
            cerr << "Failed to receive data." << endl;
            continue;
        }
        
        int index = data[0];
        copy(data + 1, data + 46081, buffer[index].begin());

        if (index == 19) {
            vector<uchar> picture;
            for (int i = 0; i < 20; i++) {
                picture.insert(picture.end(), buffer[i].begin(), buffer[i].end());
            }

            Mat frame = imdecode(picture, IMREAD_COLOR);
            if (frame.empty()) {
                cerr << "Could not decode picture." << endl;
                continue;
            }

            imshow("frame", frame);
            if (waitKey(1) == 'q') {
                break;
            }
        }
    }

    close(sock);
    return 0;
}
