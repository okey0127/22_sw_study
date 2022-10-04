#include <SoftwareSerial.h>

#define BT_RXD 9
#define BT_TXD 8
SoftwareSerial bluetooth(BT_RXD, BT_TXD);

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

}

void loop() {
  if (bluetooth.available()) {
    
    String tot = bluetooth.readStringUntil('#');
    int tot_int = tot.toInt();
    
    String key = tot.substring(0,1);
    String value = tot.substring(1);
    Serial.print(key); Serial.print(" "); Serial.println(value);
  }

}
