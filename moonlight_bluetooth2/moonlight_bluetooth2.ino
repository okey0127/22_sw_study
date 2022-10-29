#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <DS1307RTC.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Thread.h>
#include <ThreadController.h>

Thread NeoThread = Thread(); // Thread* Thread2 = new Thread(); < 포인터로 선언
ThreadController controller = ThreadController();

// 변수 선언
#define DHTPIN 2 // int DHTPIN = 2;
#define DHTTYPE DHT11

#define Backlight 3

#define Neo 4
int NEOPIXELS =  8;

// EEEPROM index
#define ld_index 0
#define ln_index 1
#define nd_index 2
#define nn_index 3
#define nR 4
#define nG 5
#define nB 6

#define BT_RXD 9
#define BT_TXD 8
SoftwareSerial bluetooth(BT_RXD, BT_TXD);

// Bluetooth 제어 모드 on/off
bool blue_flag = false;
  
// 센서 설정
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

tmElements_t tm;

Adafruit_NeoPixel pixels(NEOPIXELS, Neo, NEO_GRB + NEO_KHZ800);

int lcd_day;
int lcd_night;
int neo_day;
int neo_night;
int neo_R;
int neo_G;
int neo_B;

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
    if (neo_R == 255 && neo_G < 255 && neo_B == 0){
      neo_G++;
    }
    else if (neo_R > 0 && neo_G == 255){
      neo_R--;
    }
    else if (neo_G == 255 && neo_B < 255){
      neo_B++;
    }
    else if ( neo_G > 0 && neo_B == 255){
      neo_G--;
    }
    else if (neo_R < 255 && neo_B == 255){
      neo_R++;
    }
    else if (neo_R == 255 && neo_B > 0){
      neo_B--;
    }
    else{}
}

void setup() {
  Serial.begin(9600); // 시리얼 모니터와 9600의 보드레이트로 통신(1초당 신호 전송 속도)
  bluetooth.begin(9600);
  // EEPROM에 저장된 값 불러오기
  if (EEPROM.read(ld_index) == 0){
    // LCD 밝기
    lcd_day = EEPROM.read(ld_index);
    lcd_night = EEPROM.read(ln_index);
    // NeoPixels 밝기
    neo_day = EEPROM.read(nd_index);
    neo_night = EEPROM.read(nd_index);
  }
  else{
    lcd_day = 200;
    lcd_night = 10;
    // NeoPixels 밝기
    neo_day = 10;
    neo_night = 20;
    // NeoPixels R, G, B 값
    neo_R = 255;
    neo_G = 0;
    neo_B = 0;
  }
  
  dht.begin();

  lcd.init(); // I2C LCD를 초기화
  lcd.backlight(); // I2C LCD의 백라이트를 켬

  pinMode(Backlight, OUTPUT);

  pixels.begin(); // neopixel strip 초기화  
  
  NeoThread.setInterval(100); // 실행 간격 설정
  NeoThread.onRun(RainbowNeo);
  controller.add(&NeoThread);
}

bool RTC_flag = false;

void loop() {
  //delay(1000); // 1초 기다림(ms 단위)
  controller.run();
  float hum = dht.readHumidity();
  float temp = dht.readTemperature(); // parameter에 True를 넘기면 화씨로 나옴

  String H, M, S;

  pixels.show();
  ShowAllPixels(pixels.Color(neo_R, neo_G, neo_B));
  
  if (bluetooth.available())
  {
    String tot = bluetooth.readStringUntil('#');
    String key_ = tot.substring(0,1);
    char key = key_.charAt(0);
    String value = tot.substring(1);
    
    
    if (key == 'o'){
      blue_flag = true; 
    }
    else if (key == 'x'){
      blue_flag = false;
    }

    if (blue_flag){
      if (key == '1'){
        // brightness of lcd(day)
        lcd_day = value.toInt();
      }
      else if (key == '2'){
        // brightness of lcd(night)
        lcd_night = value.toInt();
      }
      else if (key == '3'){
        // brightness of neo(day)
        neo_day = value.toInt();
      }
      else if (key == '4'){
        // brightness of neo(night)
        neo_night = value.toInt();
      }
      else if (key == 'r'){
        neo_R = value.toInt();
      }
      else if (key == 'g'){
        neo_G = value.toInt();
      }
      else if (key == 'b'){
        neo_B = value.toInt();
      }
    }
  }
  
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
      analogWrite(Backlight, lcd_day);  
      pixels.setBrightness(neo_day);
    }
   else{
      analogWrite(Backlight, lcd_night);
      pixels.setBrightness(neo_night);
   }
  }
  
  if (isnan(hum) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!"); // print는 띄어쓰기 없고 ln 은 있음
    return;
  }
  lcd.setCursor(0, 1); // 둘째줄에 작성
  lcd.print(hum); lcd.print("% | "); lcd.print(temp); lcd.print("'C");

  EEPROM.write(ld_index, lcd_day);
  EEPROM.write(ln_index, lcd_night);
  EEPROM.write(nd_index, neo_day);
  EEPROM.write(nn_index, neo_night); 
}
