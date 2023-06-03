#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <random>

using namespace std;
using namespace cv;

#define GRID_NUM  20 // 전체 맵을 가로, 세로 얼마나 쪼갤지 (GRID_NUM x GRID_NUM)


class Packman{
    public:
        Packman(int size_width, int size_height)
        : width(size_width), height(size_height)
        {
            set_array();
            game_map = Mat(height, width, CV_8UC3, Scalar(255, 0, 0));
            pose_x = width/2;
            pose_y = height/2;
            direction_x=0;
            direction_y=0;
            offset = 2;
            angle_mouse=0;
            radius = min(width/(GRID_NUM*2)/2, height/(GRID_NUM*2)/2);
            point=0;
            draw_background();

        }

        void draw_background(){
            game_map = Mat(height, width, CV_8UC3, Scalar(255, 0, 0));
            for(int i=0 ; i<GRID_NUM ; i++){
                for(int j=0 ; j<GRID_NUM ; j++){
                    if(map_array[i][j]){
                        rectangle(  game_map , 
                                    Rect(i*(width/GRID_NUM), j*(height/GRID_NUM), width/GRID_NUM,height/GRID_NUM) , 
                                    Scalar(0, 0, 0) ,
                                    FILLED
                                );
                    }
                    
                    if(dot_array[i][j])
                        circle(game_map, Point(i*(width/GRID_NUM) + (width/GRID_NUM)/2, j*(height/GRID_NUM)+(height/GRID_NUM)/2), 5, Scalar(255, 0, 255), 1, 8, 0);

                    
                }
            }


        }
        void draw_packman(){
            ellipse(game_map, Point(pose_x,pose_y), Size(radius, radius), 0, mouthAngle+angle_mouse, 360 - mouthAngle+angle_mouse, Scalar(0, 255, 255), -1);

        }

        // 난수를 이용하여 장애물 생성 및 점 생성
        void set_array(){
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<int> uni(0, 20);
            
            for(int i=0 ; i<GRID_NUM ; i++){
                for(int j=0 ; j<GRID_NUM ; j++){
                    int bin_val =  uni(rng);
                    if(bin_val<17 ){
                        map_array[j][i] = 1;
                        dot_array[j][i] = 1;
                    }
                    else{
                        map_array[j][i] = 0;
                        dot_array[j][i] = 0;
                    }
                }
            }
            map_array[GRID_NUM/2][GRID_NUM/2]=1;
            map_array[GRID_NUM/2-1][GRID_NUM/2-1]=1;
            map_array[GRID_NUM/2][GRID_NUM/2-1]=1;
            map_array[GRID_NUM/2-1][GRID_NUM/2]=1;

            dot_array[GRID_NUM/2][GRID_NUM/2]=1;
            dot_array[GRID_NUM/2-1][GRID_NUM/2-1]=1;
            dot_array[GRID_NUM/2][GRID_NUM/2-1]=1;
            dot_array[GRID_NUM/2-1][GRID_NUM/2]=1;
        }

        bool is_inbound(int cur_x, int cur_y){
            bool return_val=true;

            return map_array[cur_x/(width/GRID_NUM)][cur_y/(height/GRID_NUM)];
        }

        bool get_map_array(int j, int i){

            return map_array[j][i];
        }

        

        void show(){

            imshow("Pacman Game", game_map);
        }

        // 팩맨 이동
        void update_pose(){
            if (direction_y == 1 && pose_y > radius){ // 위
                if( is_inbound(pose_x                   , pose_y - offset - radius) && 
                    is_inbound(pose_x - radius , pose_y - offset - radius) &&
                    is_inbound(pose_x + radius , pose_y - offset - radius))
                    pose_y -= offset; 
            }
            else if (direction_y == -1 && pose_y < game_map.rows - radius){ // 아래
                if( is_inbound(pose_x                   , pose_y + offset + radius) && 
                    is_inbound(pose_x - radius , pose_y + offset + radius) &&
                    is_inbound(pose_x + radius , pose_y + offset + radius))
                    pose_y += offset;
            }
            else if (direction_x == -1 && pose_x > radius){ // 왼쪽
                if( is_inbound(pose_x - offset - radius , pose_y                  ) && 
                    is_inbound(pose_x - offset - radius , pose_y - radius) && 
                    is_inbound(pose_x - offset - radius , pose_y + radius))
                    pose_x -= offset;
            }
            else if (direction_x == 1 && pose_x < game_map.cols - radius){ // 오른쪽
                if( is_inbound(pose_x + offset + radius , pose_y) && 
                    is_inbound(pose_x + offset + radius , pose_y - radius) && 
                    is_inbound(pose_x + offset + radius , pose_y + radius))
                    pose_x += offset;
            }

            for(int i=0 ; i<GRID_NUM ; i++){
                for(int j=0 ; j<GRID_NUM ; j++){
                    if(dot_array[i][j]){
                        if(pose_x>=i*(width/GRID_NUM) && pose_x<(i+1)*(width/GRID_NUM)){
                            if(pose_y>=j*(width/GRID_NUM) && pose_y<(j+1)*(width/GRID_NUM)){
                                dot_array[i][j] = 0;
                                point += 1;
                                cout<<"point : "<< point <<endl;
                            }
                        }
                    }
                }
            }
        }

        void update_direction(int key_val){
            // 입 벌리고 닫기
            if (openMouth){
                mouthAngle += 2;
                if (mouthAngle >= 60)
                    openMouth = false;
            }
            else{
                mouthAngle -= 2;
                if (mouthAngle <= 0)
                    openMouth = true;
            }

            // 입 방향 전환
            if(key_val == 'a' || key_val == 81){
                direction_x = -1;
                direction_y = 0;
                angle_mouse = 180;
            }
            else if (key_val == 'd' || key_val == 83){ 
                direction_x = 1;
                direction_y = 0;
                angle_mouse = 0;
            }
            else if (key_val == 'w' || key_val == 82) {
                direction_y = 1;
                direction_x = 0;
                angle_mouse = 270;
            }
            else if (key_val == 's' || key_val == 84){
                direction_y = -1;
                direction_x = 0;
                angle_mouse = 90;
            }
        }



    private:
        int width;
        int height;
        bool map_array[GRID_NUM+1][GRID_NUM+1];
        bool dot_array[GRID_NUM+1][GRID_NUM+1];
        Mat game_map;

        int pose_x;
        int pose_y;
        int direction_x;
        int direction_y;
        int offset;
        int mouthAngle = 0;
        bool openMouth = true;  
        int angle_mouse;
        int radius;
        
        int point;
};