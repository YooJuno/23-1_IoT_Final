
// g++ -o main main.cpp $(pkg-config --libs --cflags opencv4)
#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include "game.h"

using namespace std;
using namespace cv;


// 게임 화면 크기
const int size = 1000;



int main(){
    Packman packman(size);

    while (true)
    {

        int key = waitKey(10);
        if (key == 27)
            break;
        


        packman.update_direction(key);
        packman.update_pose();
        packman.draw_background();
        packman.draw_packman();
        packman.show();
        
    }
    return 0;
}


