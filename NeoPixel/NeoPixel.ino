#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
// Pin 지정
#define Neo 4
// 네오픽셀 갯수(8개)
#define NEOPIXELS 8 

Adafruit_NeoPixel pixels(NEOPIXELS, Neo, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin(); // neopixel strip 초기화
}

void loop() {
  for(int i=0; i<NEOPIXELS; i++){
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(20, 20, 20));
    pixels.show();
    delay(50);
    pixels.clear(); // 모든 픽셀 종료
  }
}
