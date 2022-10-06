#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
// Pin 지정
#define Neo 4
// 네오픽셀 갯수(8개)
#define NEOPIXELS 8 

int Neo_R = 255;
int Neo_G = 0;
int Neo_B = 0;

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
  pixels.setBrightness(100);
  ShowAllPixels(pixels.Color(Neo_R, Neo_G, Neo_B));
  if (Neo_R == 255 && Neo_G < 255){
    Neo_G++;
    delay(50);
  }
  else if (Neo_R > 0 && Neo_G == 255){
    Neo_R--;
    delay(50);
  }
  else if (Neo_G == 255 && Neo_B < 255){
    Neo_B++;
    delay(50);
  }
  else if ( Neo_G > 0 && Neo_B == 255){
    Neo_G--;
    delay(50);
  }
  else if (Neo_R < 255 && Neo_B == 255){
    Neo_R++;
    delay(50);
  }
  
}
