#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
// Pin 지정
#define Neo 4
// 네오픽셀 갯수(8개)
#define NEOPIXELS 8 

int Neo_RGB[3] = {255, 0, 0};
int pivot = 0;
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
  ShowAllPixels(pixels.Color(Neo_RGB[0], Neo_RGB[1], Neo_RGB[2]));
  
  // 아래는 무지개색을 표현하는 코드
  int index1 = (pivot + 1) % 3;

  if (Neo_RGB[index] < 255){
    Neo_RGB[index]++;
  }
  else{ // Neo_RGB[index] == 255 인 경우
    if (Neo_RGB[pivot]>0){ // 1. Neo_RGB[pivot] > 0 이면 0까지 줄인다.
      Neo_RGB[pivot]--;
    }
    else{ // 2. pivot을 바꾼다. pivot이 2보다 크면 0으로 
      pivot = (pivot+1) % 3;
    } 
  }
  delay(50);
}
