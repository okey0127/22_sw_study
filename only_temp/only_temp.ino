#include "DHT.h"

// 변수 선언
#define DHTPIN 2 // int DHTPIN = 2;
#define DHTTYPE DHT11

// 센서 설정
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(9600); // 시리얼 모니터와 9600의 보드레이트로 통신(1초당 신호 전송 속도)
  dht.begin();
}

void loop() {
  delay(1000); // 1초 기다림(ms 단위)

  float hum = dht.readHumidity();
  float temp = dht.readTemperature(); // parameter에 True를 넘기면 화씨로 나옴

  if (isnan(hum) || isnan(temp)){
    Serial.println("Failed to read from DHT sensor!"); // print는 띄어쓰기 없고 ln 은 있음
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print("% | Temperature: ");
  Serial.print(temp);
  Serial.println("°C");
}
