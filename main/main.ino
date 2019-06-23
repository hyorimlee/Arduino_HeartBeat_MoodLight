//#include <Wire.h>
//#include <RTClib.h>
#include <SoftwareSerial.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#define PROCESSING_VISUALIZER 1
#define SERIAL_PLOTTER  2
#define PERIOD 2 // 심박수 읽어오는 주기(초 단위)
#define MAXDAY 7  // 최대 저장할 수 있는 일수
#define COUNT 10  // 비정상 수치 측정하는 카운트
#define INTERVAL 60000 // 비정상 수치 측정하는 주기(1000이 1초)

//RTC_DS1307 RTC;
SoftwareSerial BTSerial(2, 3);

//  Variables
int pulsePin = A0;                 // Pulse Sensor purple wire connected to analog pin 0
int max_beat[MAXDAY] = {0, };
int min_beat[MAXDAY] = {100, };
double day_avg[MAXDAY]  = {0, };
int today = 0; // 오늘이 몇 일인지 기록
int check=0; // 현재 사용자가 어떤 상태인지 기록: 0(정상), 1(스트레스), 2(수면), 3(위험)
unsigned long time_previous, time_current;
int proBPM=0;
int MessageChk=1;

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

// SET THE SERIAL OUTPUT TYPE TO YOUR NEEDS
// PROCESSING_VISUALIZER works with Pulse Sensor Processing Visualizer
//      https://github.com/WorldFamousElectronics/PulseSensor_Amped_Processing_Visualizer
// SERIAL_PLOTTER outputs sensor data for viewing with the Arduino Serial Plotter
//      run the Serial Plotter at 115200 baud: Tools/Serial Plotter or Command+L
static int outputType = SERIAL_PLOTTER;

// function prototype

void setup () {
  Serial.begin(9600);
  //Wire.begin();
  //RTC.begin();
  //RTC.adjust(DateTime("Jun 22 2019", "20:21:30")); // 현재시간을 설정할 수 있습니다.
  interruptSetup();
  Alarm.timerRepeat(PERIOD, Repeats); // PERIOD마다 반복
  time_previous = millis();
}

//create function
void getMaxMin(void);
void getAvg(void);
void checkHeartRate(void);

void loop () {
  time_current = millis();
  //DateTime now = RTC.now();
  
  if(Serial.available())
    proBPM = Serial.parseInt();
    
  Alarm.delay(0);
}

void Repeats() {
  /*if(check == 1)
    Serial.println("stress");
  else if(check == 2)
    Serial.println("sleep");
  else if(check == 3)
    Serial.println("danger");
  else if(check == 0)
    Serial.println("normal");*/ 
  // 시리얼 모니터로 상태 확인
  
  Serial.println(String("proBPM: ") + proBPM);
  getMaxMin();
  getAvg();
  checkHeartRate();
  if(check == 3 && Messagechk==1) { 
    BTSerial.write('3');
    Messagechk=0;
  }
  neo(check);
  
  /*if(QS == true) {
    Serial.println(String("BPM: ") + BPM);
    getMaxMin();
    getAvg();
    checkHeartRate();
    neo(check);
    QS = false;
  }*/
  // 심박센서로 BPM 지수 받아서 처리하기
}

void getMaxMin(void)
{
  if( proBPM > max_beat[today] )
    max_beat[today] = proBPM;
  if( proBPM < min_beat[today] )
    min_beat[today] = proBPM;
  Serial.println(String("MAX: ") + max_beat[today] + String(" MIN: ") + min_beat[today]);
}

void getAvg(void)
{
  static int sum=0, avgCount=0;
  sum += proBPM;
  avgCount++;
  day_avg[today] = (double)sum/avgCount;
  Serial.println(String("AVG: ") + day_avg[today]);
  Serial.println("========================================================");
}

void checkHeartRate(void)
{
  static int stressCount=0, sleepCount=0, dangerCount=0, normalCount=0;

  if(time_current - time_previous >= INTERVAL) {
    time_previous = time_current;
    stressCount=0;
    sleepCount=0;
    dangerCount=0;
  }

  if( proBPM > 85)
    stressCount++;
  else if( proBPM > 60 && proBPM <=85)
    normalCount++;
  else if( proBPM >= 30 && proBPM <= 60)
    sleepCount=0;
  else if( proBPM < 30)
    dangerCount++;

  if(stressCount >= COUNT){
    check=1;
    Messagechk=1;
  }
  else if(sleepCount >= COUNT){
    check=2;
    Messagechk=1;
  }
  else if(dangerCount >= COUNT )
    check=3;
  else if(normalCount >= COUNT){
    check=0;
    Messagechk=1;
  }
}
