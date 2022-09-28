#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // I2C LCD를 초기화
  lcd.init();
  // I2C LCD의 백라이트를 켬
  lcd.backlight();
}

void loop() { 
  lcd.setCursor(0,0);       // 표시할 문자의 시작 위치 설정   
  lcd.print("Hello World!"); // 영어, 숫자, 특수문자만 됨
}
