// g++ -o main main.cpp $(pkg-config --libs --cflags opencv4) -lpthread -lboost_system
#include <iostream>
#include <opencv4/opencv2/opencv.hpp>

using namespace std;
using namespace cv;


// 게임 화면 크기 및 초기 팩맨 위치 설정
const int screenWidth = 500;
const int screenHeight = 500;





int is_inbound(int x, int y, int map[][10]);

int main(){

    int map[10][10] = {
        { 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 },
        { 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 },
        { 1 , 0 , 0 , 1 , 0 , 1 , 1 , 0 , 0 , 1 },
        { 1 , 0 , 1 , 1 , 1 , 1 , 1 , 0 , 0 , 1 },
        { 1 , 1 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 1 },
        { 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 },
        { 1 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 },
        { 1 , 0 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 0 },
        { 1 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 1 },
        { 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 }
    };

    Point PacmanPosition(screenWidth / 2, screenHeight / 2);

    // 입의 초기 각도 및 방향 설정
    int mouthAngle = 0;
    bool openMouth = true;

    int radius = (screenWidth/20)/2;

    // 게임 윈도우 생성
    namedWindow("Pacman Game", WINDOW_NORMAL);
    resizeWindow("Pacman Game", screenWidth, screenHeight);

    int direction_x = 0;    // left =
     -1    ,     right =  1
    int direction_y = 0;    // up   =  1    ,     down  = -1
    int angle_mouse = 0;

    int offset = 2;


    while (true)
    {
        // 키 입력을 받습니다.
        int key = waitKey(10);
        
        
        // ESC 키를 누르면 프로그램을 종료합니다.
        if (key == 27)
            break;
        
        // 이전 팩맨 그리기
        Mat gameScreen(screenHeight, screenWidth, CV_8UC3, Scalar(255, 0, 0));
        // ellipse(gameScreen, PacmanPosition, Size(30, 30), 0, mouthAngle, 360 - mouthAngle, Scalar(255, 255, 0), -1);


        // 입 각도 업데이트
        if (openMouth)
        {
            mouthAngle += 2;
            if (mouthAngle >= 60)
                openMouth = false;
        }
        else
        {
            mouthAngle -= 2;
            if (mouthAngle <= 0)
                openMouth = true;
        }

        if      (key == 'a' || key == 81){
            direction_x = -1;
            direction_y = 0;
            angle_mouse = 180;
        }
        else if (key == 'd' || key == 83){ 
            direction_x = 1;
            direction_y = 0;
            angle_mouse = 0;
        }
        else if (key == 'w' || key == 82) {
            direction_y = 1;
            direction_x = 0;
            angle_mouse = 270;
        }
        else if (key == 's' || key == 84){
            direction_y = -1;
            direction_x = 0;
            angle_mouse = 90;
        }

        // // 키 입력에 따라 원의 위치를 업데이트합니다.
        if (direction_y == 1 && PacmanPosition.y > radius){ // 위
            if(is_inbound(PacmanPosition.x , PacmanPosition.y - offset - radius, map))
                PacmanPosition.y -= offset; 
        }
        else if (direction_y == -1 && PacmanPosition.y < gameScreen.rows - radius){ // 아래
            if(is_inbound(PacmanPosition.x , PacmanPosition.y + offset + radius, map))
                PacmanPosition.y += offset;
        }
        else if (direction_x == -1 && PacmanPosition.x > radius){ // 왼쪽
            if(is_inbound(PacmanPosition.x - offset - radius , PacmanPosition.y, map))
                PacmanPosition.x -= offset;
        }
        else if (direction_x == 1 && PacmanPosition.x < gameScreen.cols - radius){ // 오른쪽
            if(is_inbound(PacmanPosition.x + offset + radius , PacmanPosition.y, map))
                PacmanPosition.x += offset;
        }

        // 업데이트된 팩맨 그리기
        for(int i=0 ; i<10 ; i++)
            for(int j=0 ; j<10 ; j++)
                if(map[j][i])
                    rectangle(gameScreen , Rect(i*(screenWidth/10),j*(screenHeight/10),screenWidth/10,screenHeight/10) , Scalar(0, 0, 0) ,FILLED);

        ellipse(gameScreen, PacmanPosition, Size(radius, radius), 0, mouthAngle+angle_mouse, 360 - mouthAngle+angle_mouse, Scalar(0, 255, 255), -1);
        // 업데이트된 화면을 표시합니다.
        imshow("Pacman Game", gameScreen);

        for(int i=0 ; i<10 ; i++){
            for(int j=0 ; j<10 ; j++){
                
            }
        }
        
    }

    waitKey(0);

    return 0;
}



int is_inbound(int x, int y, int map[][10]){

    

    int map_x = x/(screenWidth/10);
    int map_y = y/(screenHeight/10);



    return map[map_y][map_x];
}
