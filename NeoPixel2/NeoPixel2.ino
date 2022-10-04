#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
// Pin 지정
#define Neo 4
// 네오픽셀 갯수(8개)
#define NEOPIXELS 8 

Adafruit_NeoPixel pixels(NEOPIXELS, Neo, NEO_GRB + NEO_KHZ800);

void ShowAllPixels(uint32_t color){
  for(int i=0; i<NEOPIXELS; i++){
    pixels.setPixelColor(i, color);
  }
}
void setup() {
  pixels.begin(); // neopixel strip 초기화
}

void loop() {
  pixels.show();
  pixels.setBrightness(10);
  ShowAllPixels(pixels.Color(250, 120, 58));
}
