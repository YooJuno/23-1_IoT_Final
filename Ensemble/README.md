# 온라인 합주 프로그램

**기본 개념**

* Protocol : "VoIP(Voice Over Internet Protocol)
* 여기서도 RTP개념은 사용해야함. 이유는 VoIP또한 패킷의 순서보장을 하지 못하기 때문.


* 대개 합주에 있어서 사람이 느끼는 레이턴시는 10ms~14ms정도임. 이 시간 안에 클라이언트간 전송이 되어야함.


* 이 때 Propagation Delay와 Transmission Delay를 신경써야함
  1. Propagation Delay : 로컬에서 실시할거라 최대 2개홉임. 즉 전파지연은 걱정 안해도 될듯.
  2. Transmission Delay : 아무리 신경써도 줄이는데 한계가 있음. 즉 애초에 전송할 데이터 양을 줄여야함.
     * 이를 위해 VoIP와 같은 음성 압축 프로토콜 및 API등의 기술 사용필요.
