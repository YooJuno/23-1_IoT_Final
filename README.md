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
https://velog.io/@minukiki/Ubuntu-20.04%EC%97%90-OpenCV-4.4.0-%EC%84%A4%EC%B9%98

### BOOST 라이브러리 설치 방법
https://yaaam.tistory.com/entry/Ubuntu-Boost-%EC%84%A4%EC%B9%98-Ubuntu-%ED%99%98%EA%B2%BD%EC%97%90%EC%84%9C-Boost-%EC%84%A4%EC%B9%98%EB%B0%A9%EB%B2%95


# 사용 방법

### Git Repository Clone

$ git clone https://github.com/YooJuno/IoT_Final.git 

### Server.cpp Compile

$ g++ -std=c++11 Server.cpp -o Server -lboost_system -pthread  

### Client.cpp Compile

$ g++ -std=c++11 Client.cpp -o Client -lboost_system -pthread $(pkg-config --libs --cflags opencv4)  

### Game 실행 방법

##### ./Server <ip_address> <port> 순서대로 Command에 작성한다.  
<img width="556" alt="KakaoTalk_20230614_204955234" src="https://github.com/YooJuno/IoT_Final/assets/82189642/e552e56b-32d8-4c8f-9a00-5edb3527e921">
  
##### ./Client <ip_address> <port> 순서대로 Command에 작성하여 Client 코드를 실행시킨다.  
<img width="558" alt="KakaoTalk_20230614_205023248" src="https://github.com/YooJuno/IoT_Final/assets/82189642/e290e988-a84e-4ab5-9878-1fd367462a39">
  
##### Server 쪽에 사용자 정보가 다음처럼 등록된다.
<img width="546" alt="KakaoTalk_20230614_205032602" src="https://github.com/YooJuno/IoT_Final/assets/82189642/7a1d92a4-5c59-44ee-bfad-11066db71955">

<img width="562" alt="KakaoTalk_20230614_205133305" src="https://github.com/YooJuno/IoT_Final/assets/82189642/16acbcb8-872e-4c9b-a3d8-9d8731ba963d">
  
##### 4명의 Client가 접속하면 게임이 실행된다.
<img width="378" alt="KakaoTalk_20230614_205204755" src="https://github.com/YooJuno/IoT_Final/assets/82189642/2f4f232c-69c3-4f36-93c2-bfd963854968">
