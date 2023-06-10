// g++ -o main main.cpp $(pkg-config --libs --cflags opencv4)
#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include "game.h"

using namespace std;
using namespace cv;

int main(){
    
    const int size = 600; // Size of the game map
    
    Background background(size);
    Pacman pacman1(size, background, 1);
    Pacman pacman2(size, background, 2);
    Pacman pacman3(size, background, 3);
    Pacman pacman4(size, background, 4);

    namedWindow("Pacman Game", WINDOW_NORMAL);
    resizeWindow("Pacman Game", size, size);

    while (true) {
        char key = waitKey(10);
        //if (key == 'q' || key == 'Q')
        //    break;

        background.draw_background();
        pacman1.draw_packman();
        pacman2.draw_packman();
        pacman3.draw_packman();
        pacman4.draw_packman();
        imshow("Pacman Game", background.game_map);

        pacman1.update_direction(key);
        pacman2.update_direction(81);
        pacman3.update_direction(82);
        pacman4.update_direction(83);

        pacman1.update_pose();
        pacman2.update_pose();
        pacman3.update_pose();
        pacman4.update_pose();
    }

    destroyAllWindows();
    return 0;
}
