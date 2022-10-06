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
int neo_R = 250;
int neo_G = 120;
int neo_B = 50;

  
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

void setup() {
  Serial.begin(9600); // 시리얼 모니터와 9600의 보드레이트로 통신(1초당 신호 전송 속도)
  dht.begin();

  lcd.init(); // I2C LCD를 초기화
  lcd.backlight(); // I2C LCD의 백라이트를 켬

  pinMode(Backlight, OUTPUT);

  pixels.begin(); // neopixel strip 초기화

}

bool RTC_flag = false;

void loop() {
  delay(1000); // 1초 기다림(ms 단위)

  float hum = dht.readHumidity();
  float temp = dht.readTemperature(); // parameter에 True를 넘기면 화씨로 나옴

  String H, M, S;

  pixels.show();
  ShowAllPixels(pixels.Color(neo_R, neo_G, neo_B));
  
  if (RTC.read(tm)) {
   H = alt2digits(tm.Hour);
   M = alt2digits(tm.Minute);
   S = alt2digits(tm.Second);
   String Year = String(tmYearToCalendar(tm.Year)).substring(2,4);
   String Mon = alt2digits(tm.Month);
   lcd.setCursor(1, 0); // 첫째줄에 한칸 띄워 작성
   lcd.print(Year); lcd.print('/'); lcd.print(Mon); lcd.print('/'); lcd.print(tm.Day); lcd.print(' ');
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
    pixels.setBrightness(2);
   }
  }
  
  if (isnan(hum) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!"); // print는 띄어쓰기 없고 ln 은 있음
    return;
  }
  lcd.setCursor(0, 1); // 둘째줄에 작성
  lcd.print(hum); lcd.print("% | "); lcd.print(temp); lcd.print("'C");

}
