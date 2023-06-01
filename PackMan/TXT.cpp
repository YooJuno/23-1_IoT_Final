#include <opencv2/opencv.hpp>

int main()
{
    // 게임 화면 크기 및 초기 팩맨 위치 설정
    const int screenWidth = 800;
    const int screenHeight = 600;
    cv::Point pacmanPosition(screenWidth / 2, screenHeight / 2);

    // 입의 초기 각도 및 방향 설정
    int mouthAngle = 0;
    bool openMouth = true;

    // 게임 윈도우 생성
    cv::namedWindow("Pacman Game", cv::WINDOW_NORMAL);
    cv::resizeWindow("Pacman Game", screenWidth, screenHeight);

    while (true)
    {
        // 키 입력 받기
        int key = cv::waitKey(1);

        // ESC 키를 누르면 게임 종료
        if (key == 27)
            break;

        // 이전 팩맨 그리기
        cv::Mat gameScreen(screenHeight, screenWidth, CV_8UC3, cv::Scalar(0, 0, 0));
        cv::ellipse(gameScreen, pacmanPosition, cv::Size(30, 30), 0, mouthAngle, 360 - mouthAngle, cv::Scalar(255, 255, 0), -1);

        // 입 각도 업데이트
        if (openMouth)
        {
            mouthAngle += 5;
            if (mouthAngle >= 60)
                openMouth = false;
        }
        else
        {
            mouthAngle -= 5;
            if (mouthAngle <= 0)
                openMouth = true;
        }

        // 키 입력에 따라 팩맨의 위치 업데이트
        switch (key)
        {
        case 'w':
            if (pacmanPosition.y > 30)
                pacmanPosition.y -= 10;
            break;
        case 's':
            if (pacmanPosition.y < screenHeight - 30)
                pacmanPosition.y += 10;
            break;
        case 'a':
            if (pacmanPosition.x > 30)
                pacmanPosition.x -= 10;
            break;
        case 'd':
            if (pacmanPosition.x < screenWidth - 30)
                pacmanPosition.x += 10;
            break;
        }

        // 업데이트된 팩맨 그리기
        cv::ellipse(gameScreen, pacmanPosition, cv::Size(30, 30), 0, mouthAngle, 360 - mouthAngle, cv::Scalar(255, 255, 0), -1);

        // 게임 화면 표시
        cv::imshow("Pacman Game", gameScreen);
    }

    // 게임 종료 후 윈도우 닫기
    cv::destroyWindow("Pacman Game");

    return 0;
}
