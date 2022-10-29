unsigned long time_previous, time_current;

void setup() {
  time_previous = millis();
  Serial.begin(9600);
}
int cnt = 0;
void loop() {
  time_current = millis();

  unsigned long diff = 0;
  diff = time_current - time_previous;
  if (diff >= 1000){
    cnt += 1; // 카운트 
    time_previous = time_current; // 시간 갱신
    Serial.print(diff); Serial.print(' '); Serial.println(cnt);
  }

}
