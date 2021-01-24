# Arduino_HeartBeat_MoodLight / 아두이노-건강무드등
Arduino Mega, Neopixel ring, Bluetooth(HC 06 slave), Wifi modue(esp 8266)

블루투스 모듈을 통해 사용자의 심박수 데이터 수집.
수집된 심박수 데이터를 바탕으로 사용자의 상태를 파악하여 적절하게 무드등의 색 변화.
심장마비와 같이 심박수 이상 상태의 경우 비상 문자 전송.

## Functions / 기능
**심박수를 통해 사용자의 상태를 파악 하여 무드등의 색 변화**
* 정상 상태 : 높은 채도
* 스트레스 상태 : 낮은 채도의 부드러운 색
* 수면 상태 : 등 꺼짐
* 위험 상태(심박수 이상) : 빨간색 경고등

**Monitoring heart beat of user / 심박수 실시간 모니터링**
* 어플을 통해  심박수 확인 가능
* 위험 상태(심박수 이상) 인식시 미리 지정 된 번호에 비상 문자 전송

## 작동영상
**https://drive.google.com/file/d/1DnTzn93he938tZDGJ_mQw-_dNV6jwcfr/view?usp=sharing**

