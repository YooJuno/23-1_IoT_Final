# IoT 실습 FInal Project 계획서

**21800471 유준호**

**21800201 김현욱**

### [주제] : Pac-Man Game

**구현 환경**

* UBUNTU18.04 이상
* OPENCV4.4.0 이상 (C++)
* boost asio

**[세부 구현 내용]**

![](https://lh4.googleusercontent.com/out7KENUb-CTDQq3jeB8yBvxrg53YCLAPPtaGBNyGeH7ITGgi9g1PLWp8OjjRDRJYHccqO9ASNS-3W3xwmiBImR1SWj9ZlvjP_JB4gp5B45AxwGjUgqiePhE2E7_Eln3v-1L1gOmuAB5jaq9aeGx_-M)

1980년 남코회사에서 출시된 Pac-Man 게임은 단순한 구조로 많은 사람들에게 사랑을 받은 플래시 게임입니다. 기본적으로 pac-man게임은 1인용 게임으로 귀신을 피해다니며 맵에 퍼져있는 점을 먹으며 점수를 쌓아가는 게임입니다.

저희가 구현하는 프로그램은 동시에 다중 사용자들이 이용할 수 있도록 하는것이 목표입니다. 서버에 게임방을 생성합니다. 이 때 클라이언트 게임 속도와 인원을 설정하여, 정원이 차게되면 게임을 시작합니다. 모든 클라이언트는 방을 생성할 수 있습니다. 게임이 시작한 후 클라이언트가 보게될 화면은 좌측의 사진에서 다른 클라이언트의 pac-man이 추가된 화면을 볼 수 있습니다. 각각의 클라이언트는 맵을 돌아다니며 점수를 획득합니다. 모든 점이 없어진 그 순간의 점수를 토대로 결과 화면을 클라이언트에게 보여줍니다.

서버와 클라이언트 측면에서 살펴보자면

**[Server]**

서버는 클라이언트의 입장, 퇴장을 담당하며 게임에 대한 정보를 갖고있습니다. 이 때 게임에 대한 정보는 맵의 지형, 장애물 등 게임 진행에 필요한 임의의 초기 설정값을 의미합니다. 이러한 초기 세팅값은 게임이 시작된 순간에 클라이언트에게 송신되며, 그 이후에는 송신되지 않습니다. 이 후 각 클라이언트로 부터 팩맨의 위치 정보를 비롯한 정보들을 지속적으로 입력받아 다른 클라이언트에게 보내주는 채팅 서버의 역할을 하게됩니다. 이 때 서버는 비동기적으로 패킷을 송수신하여 게임의 원활한 진행을 만들어줍니다. boost asio의 비동기 입출력을 사용할 예정입니다.

**[Client]**

클라이언트는 초기 게임방 생성, 참가 후 서버로부터 맵 정보를 수신합니다. 이 후 사용자는 방향키를 비롯한 게임 진행에 필요한 키보드 입력을 줍니다. 입력된 키보드 값으로 알게된 pac-man의 위치값, 방향, 점수, 등을 서버에 지속적으로 송신합니다. 이와 동시에 비 동기적으로 서버로부터 다른 사용자의 위치, 방향, 점수들을 수신합니다. 이를 이용하여 다른 사용자 pac-man을 화면상에 출력하여 같이 게임하고 있다는 느낌을 사용자에게 제공합니다.

# 구현 환경 설정

### OPENCV4 설치 방법

출처 : https://github.com/YooJuno/IoT_Final

## 1. 설치된 OpenCV 제거

전에 설치했던 OpenCV가 있다면 새로 설치하는 OpenCV 4.4.0 버전이 제대로 동작하지 않기 때문에 제거해주어야 합니다.

아래처럼 보이면 OpenCV가 설치안되어 있는 상태입니다.

```null
$ pkg-config --modversion opencv
```

![](https://media.vlpt.us/images/minukiki/post/faa18dea-3feb-4bf0-b6e0-35116e34801d/Screenshot%20from%202022-04-02%2008-56-57.png)

설치되어 있는 경우에는 버전이 출력이 됩니다.
다음 명령으로 OpenCV 설정 파일을 포함해서 패키지를 삭제하고 진행해야 합니다.

```null
$ sudo apt-get purge  libopencv* python-opencv
$ sudo apt-get autoremove
```

## 2. 기존 설치된 패키지 업그레이드

OpenCV 4.4.0을 설치해주기 전에 기존에 설치된 패키지들을 업그레이드 해주기 위한 작업입니다.

Ubuntu 저장소로부터 패키지 리스트를 업데이트합니다.
기존에 설치된 패키지들의 새로운 버전이 저장소에 있다면 리스트를 업데이트 하기위해 실행합니다.

```null
$ sudo apt-get update
$ sudo apt-get upgrade
```

## 3. OpenCV 컴파일 전 필요한 패키지 설치

컴파일 하는데 사용하는 것들이 포함된 패키지들을 설치합니다. 이미 설치된 경우도 있습니다.

**bulid-essential** 패키지는 C/C++ 컴파일러와 관련 라이브러리, make 같은 도구들이 포함되어 있습니다.
**cmake**는 컴파일 옵셥이나 빌드된 라이브러리에 포함 시킬 OpenCV 모듈 설정등을 위해 필요합니다.

```null
$ sudo apt-get install build-essential cmake
```

**pkg-config**는 프로그램 컴파일 및 링크시 필요한 라이브러리에 대한 정보를 메타파일(확장자가 .pc 인 파일)로부터 가져오는데 사용됩니다.

터미널에서 특정 라이브러리를 사용한 소스코드를 컴파일시 필요한 헤더파일 및 라이브러리 위치 및 옵션을 추가하는데 도움이 됩니다.

```null
$ sudo apt-get install pkg-config
```

![](https://media.vlpt.us/images/minukiki/post/d81806b1-fe2b-476e-8224-b8b25462cef0/Screenshot%20from%202022-04-02%2009-30-24.png)

OpenGL 지원하기 위해 필요한 라이브러리입니다.

> ```null
> $ sudo apt-get install mesa-utils libgl1-mesa-dri libgtkgl2.0-dev libgtkglext1-dev
> ```

OpenCV 최적화를 위해 사용되는 라이브러리들입니다.

> ```null
> $ sudo apt-get install libatlas-base-dev gfortran libeigen3-dev
> ```

python3-dev 패키지는 OpenCV-Python 바인딩을 위해 필요한 패키지들입니다.

Numpy는 매트릭스 연산등을 빠르게 처리할 수 있어서 OpenCV에서 사용됩니다.

> ```null
> $ sudo apt-get install python3-dev python3-numpy
> ```

## 4. OpenCV 설정과 컴파일 및 설치

소스 코드를 저장할 임시 디렉토리를 생성하여 이동 후 진행합니다.

> ```null
> $ mkdir opencv
> $ cd opencv
> ```
>
> ![](https://media.vlpt.us/images/minukiki/post/ebe72e2b-1862-4456-86a5-1a63ba2905fe/Screenshot%20from%202022-04-02%2009-43-09.png)

OpenCV 4.4.0 소스코드를 다운로드 받아 압축을 풀어줍니다.

> ```null
> $ wget -O opencv.zip https://github.com/opencv/opencv/archive/4.4.0.zip
> $ unzip opencv.zip
> ```

opencv_contrib(Extra modules) 소스코드를 다운로드 받아 압축을 풀어줍니다.

기본 모듈에서 빠진 모듈들과 SURF 등의 nonfree 모듈을 사용하기 위해 필요합니다.

SIFT는 OpenCV 4.4.0부터 Extra 모듈에서 기본 모듈로 옮겨졌습니다.

> ```null
> $ wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.4.0.zip
> $ unzip opencv_contrib.zip
> $ ls
> ```
>
> 다음처럼 두개의 디렉토리가 생성됩니다.
> ![](https://media.vlpt.us/images/minukiki/post/463d1991-ffd1-4aec-b15b-29a560bd88d3/Screenshot%20from%202022-04-02%2009-48-43.png)

opencv-4.4.0 디렉토리로 이동하여 build 디렉토리를 생성하고 build 디렉토리로 이동합니다.

컴파일은 build 디렉토리에서 이루어집니다.

> ```null
> $ cd opencv-4.4.0
> $ mkdir bulid
> $ cd bulid
> ```
>
> ![](https://media.vlpt.us/images/minukiki/post/991c4dd7-888f-42a9-8622-091f75b99e35/Screenshot%20from%202022-04-02%2009-50-57.png)

cmake를 사용하여 OpenCV 컴파일 설정을 해줍니다.

```opencv

그래서 OPENCV_GENERATE_PKGCONFIG=ON 옵션을 추가했습니다. 

Non free 모듈을 사용하려면 다음 옵션을 추가했습니다.

( 참고 https://github.com/opencv/opencv/issues/13154 ) 

-D OPENCV_ENABLE_NONFREE=ON
```

> cmake가 없다면 `$ sudo apt install cmake`
>
> ```null
> $ cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=OFF -D WITH_IPP=OFF -D WITH_1394=OFF -D BUILD_WITH_DEBUG_INFO=OFF -D BUILD_DOCS=OFF -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON -D BUILD_EXAMPLES=OFF -D BUILD_PACKAGE=OFF -D BUILD_TESTS=OFF -D BUILD_PERF_TESTS=OFF -D WITH_QT=OFF -D WITH_GTK=ON -D WITH_OPENGL=ON -D BUILD_opencv_python3=ON -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-4.4.0/modules -D WITH_V4L=ON  -D WITH_FFMPEG=ON -D WITH_XINE=ON -D OPENCV_ENABLE_NONFREE=ON -D BUILD_NEW_PYTHON_SUPPORT=ON -D OPENCV_SKIP_PYTHON_LOADER=ON -D OPENCV_GENERATE_PKGCONFIG=ON ../
> ```
>
> 다음과 같은 메시지가 보이면 정상적으로 된 것입니다.
> ![](https://media.vlpt.us/images/minukiki/post/b689b4d7-1c3e-4742-b595-aee700c91981/Screenshot%20from%202022-04-02%2009-58-28.png)

> cmake를 사용하여 진행한 OpenCV컴파일 관련 설정 결과입니다.
> ![](https://media.vlpt.us/images/minukiki/post/b2747e99-25fc-47b8-8540-3ede437496a5/Screenshot%20from%202022-04-02%2010-08-36.png)
> ![](https://media.vlpt.us/images/minukiki/post/bdd143e6-f05d-4a7b-a5ca-d77a09c9d556/Screenshot%20from%202022-04-02%2010-08-59.png)
> ![](https://media.vlpt.us/images/minukiki/post/32096002-3513-4bde-82e6-0be33e954959/Screenshot%20from%202022-04-02%2010-09-10.png)

다음처럼 Python 3 라이브러리 항목이 보이지 않는 경우에는 ( 저는 보였기 때문에 하지 않았습니다. )

굵은 글처럼 해당 경로들을 직접 적어줘야 합니다.포스팅에서 사용한 옵션과 차이가 있을 수 있습니다. ( 위에서 말했듯이 저는 하지 않았습니다. 그러므로 차이가 있을 수 있습니다.)

> 위의 cmake 명령어 뒤에 붙혀 쓰면 됩니다.
> **-D PYTHON3_INCLUDE_DIR=/usr/include/python3.8 -D PYTHON3_NUMPY_INCLUDE_DIRS=/usr/lib/python3/dist-packages/numpy/core/include/ -D PYTHON3_PACKAGES_PATH=/usr/lib/python3/dist-packages -D PYTHON3_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.8.so ../**

make 명령을 사용하여 컴파일을 시작합니다. 앞에 time을 붙여서 실행하면 컴파일 완료 후 걸린 시간을 알려줍니다.

> ```null
> $ time make -j$(nproc)
> ```

컴파일 성공하면 다음과 같은 메시지를 볼 수 있습니다.

![](https://media.vlpt.us/images/minukiki/post/95bb24a2-c602-48f3-a6d2-cf01f1568b68/Screenshot%20from%202022-04-02%2010-17-57.png)

이제 컴파일 결과물을 설치합니다.

> ```null
> $ sudo make install
> ```

/etc/ld.so.conf.d/ 디렉토리에 /usr/local/lib를 포함하는 설정파일이 있는지 확인합니다.

> ```null
> $ cat /etc/ld.so.conf.d/*
> ```
>
> ![](https://media.vlpt.us/images/minukiki/post/4ef8927f-9ba7-4190-a8aa-d8181299a937/Screenshot%20from%202022-04-02%2010-20-29.png)

/usr/local/lib이 출력되지 않았다면 다음 명령을 추가로 실행해야합니다. ( 저는 추가 되어있습니다. )

> ```null
> $ sudo sh -c 'echo '/usr/local/lib' > /etc/ld.so.conf.d/opencv.conf'
> ```

/usr/local/lib을 찾은 경우나 못찾아서 추가한 작업을 한 경우 모두 컴파일시 opencv 라이브러리를 찾을 수 있도록 다음 명령을 실행합니다.

> ```null
> $ sudo ldconfig
> ```

### BOOST 라이브러리 설치 방법

다운로드 : [https://www.boost.org/users/download/](https://www.boost.org/users/download/)

위 사이트에서 최신 boost를 다운로드받습니다.

(여기에서는 tar.gz을 기준으로 설명합니다.)

다음 순서대로 진행합니다.

$ tar xvfz boost_1_72_0.tar.gz

$ cd boost_1_72_0

$ sudo ./bootstrap.sh

$ sudo ./b2 install

정상적으로 완료되었다면 boost를 사용하는 소스 코드의 경고 소멸 및 실행 파일이 정상적으로 동작하는 것을 확인할 수 있습니다.

# 사용 방법

### Git Repository Clone

$ git clone https://github.com/YooJuno/IoT_Final.git

### Server.cpp Compile

$ g++ -std=c++11 Server.cpp -o Server -lboost_system -pthread

### Client.cpp Compile

$ g++ -std=c++11 Client.cpp -o Client -lboost_system -pthread $(pkg-config --libs --cflags opencv4)

### Game 실행 방법

##### ./Server <ip_address> `<port>` 순서대로 Command에 작성한다.

<img width="556" alt="KakaoTalk_20230614_204955234" src="https://github.com/YooJuno/IoT_Final/assets/82189642/e552e56b-32d8-4c8f-9a00-5edb3527e921">

##### ./Client <ip_address> `<port>` 순서대로 Command에 작성하여 Client 코드를 실행시킨다.

<img width="558" alt="KakaoTalk_20230614_205023248" src="https://github.com/YooJuno/IoT_Final/assets/82189642/e290e988-a84e-4ab5-9878-1fd367462a39">

##### Server 쪽에 사용자 정보가 다음처럼 등록된다.

<img width="546" alt="KakaoTalk_20230614_205032602" src="https://github.com/YooJuno/IoT_Final/assets/82189642/7a1d92a4-5c59-44ee-bfad-11066db71955">

<img width="562" alt="KakaoTalk_20230614_205133305" src="https://github.com/YooJuno/IoT_Final/assets/82189642/16acbcb8-872e-4c9b-a3d8-9d8731ba963d">

##### 4명의 Client가 접속하면 게임이 실행된다.

<img width="378" alt="KakaoTalk_20230614_205204755" src="https://github.com/YooJuno/IoT_Final/assets/82189642/2f4f232c-69c3-4f36-93c2-bfd963854968">
