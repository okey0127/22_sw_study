#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <DS1307RTC.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// 변수 선언
#define DHTPIN 2 // int DHTPIN = 2;
#define DHTTYPE DHT11

#define Backlight 3

#define Neo 4
#define NEOPIXELS 8 

// NeoPixels R, G, B 값
int Neo_RGB[3] = [255, 0, 0];
int pivot = 0;
  
// 센서 설정
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

tmElements_t tm;

Adafruit_NeoPixel pixels(NEOPIXELS, Neo, NEO_GRB + NEO_KHZ800);

//functions
String alt2digits(int number) {
  // tm.Hour, Minute ... 등 tm 라이브러리의 명령어 출력 결과는 1 -> 1 로 나옵니다.
  // 보기 좋게 출력하기 위하 1 -> 01 등으로 출력 되도록 number 가 0~9 의 값이면 number앞에 0을 붙여줍니다.
  // 01 과 같이 형태를 유지하기 위해 number의 type을 string로 바꿉니다.
  String num;
  if (number >= 0 && number < 10) {
    num = '0' + String(number);
  }
  else{
    num = String(number);
  }
  return num;
}

void ShowAllPixels(uint32_t color){
  for(int i=0; i<NEOPIXELS; i++){
    pixels.setPixelColor(i, color);
  }
}

void RainbowNeo(){
  int index1 = pivot + 1;
  if (index > 2){
    index = 0;
  }

  if (Neo_RGB[index] < 255){
    Neo_RGB[index]++;
  }
  else{ // Neo_RGB[index] == 255 인 경우
    if (Neo_RGB[pivot]>0){ // 1. Neo_RGB[pivot] > 0 이면 0까지 줄인다.
      Neo_RGB[pivot]--;
    }
    else{ // 2. pivot을 바꾼다. pivot이 2보다 크면 0으로 
      pivot++;
      if (pivot>2){
        pivot = 0;
      }
    } 
  }
}
void setup() {
  Serial.begin(9600); // 시리얼 모니터와 9600의 보드레이트로 통신(1초당 신호 전송 속도)
  dht.begin();

  lcd.init(); // I2C LCD를 초기화
  lcd.backlight(); // I2C LCD의 백라이트를 켬

  pinMode(Backlight, OUTPUT);

  pixels.begin(); // Neopixel strip 초기화
}

bool RTC_flag = false;

void loop() {
  float hum = dht.readHumidity();
  float temp = dht.readTemperature(); // parameter에 True를 넘기면 화씨로 나옴

  String H, M, S;

  pixels.show();
  ShowAllPixels(pixels.Color(Neo_RGB[0], Neo_RGB[1], Neo_RGB[2]));
  RainbowNeo();
  if (RTC.read(tm)) {
   H = alt2digits(tm.Hour);
   M = alt2digits(tm.Minute);
   S = alt2digits(tm.Second);
   String Year = String(tmYearToCalendar(tm.Year)).substring(2,4);
   String Mon = alt2digits(tm.Month);
   String Day = alt2digits(tm.Day);
   lcd.setCursor(1, 0); // 첫째줄에 한칸 띄워 작성
   lcd.print(Year); lcd.print('/'); lcd.print(Mon); lcd.print('/'); lcd.print(Day); lcd.print(' ');
   lcd.print(H);  lcd.print(':'); lcd.print(M); 
   RTC_flag = true;
  }
  if (RTC_flag) {
    int Hour;
    Hour = H.toInt();
    
    if (Hour > 6 && Hour < 18) {
      analogWrite(Backlight, 255);  
      pixels.setBrightness(100);
    }
   else{
    analogWrite(Backlight, 15);
    pixels.setBrightness(20);
   }
  }
  
  if (isnan(hum) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!"); // print는 띄어쓰기 없고 ln 은 있음
    return;
  }
  lcd.setCursor(0, 1); // 둘째줄에 작성
  lcd.print(hum); lcd.print("% | "); lcd.print(temp); lcd.print("'C");
  
}
