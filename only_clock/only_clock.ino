#include <Wire.h>
#include <DS1307RTC.h>

const char *monthName[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

tmElements_t tm;

// functions
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


bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}


bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex; // unsigned char 0~255 의 값
  
  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false; // sscanf는 변수 갯수를 return
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1; // 숫자로 저장해야 됨
  tm.Year = CalendarYrToTm(Year);
  return true;
}

void setup() {
  Serial.begin(9600);
  while (!Serial); // wait for serial
  delay(200);
  
  // __DATE__: 컴파일 된 날짜, __TIME__: 컴파일 된 시간을 RTC 모듈에 저장
  getDate(__DATE__);
  getTime(__TIME__);
  RTC.write(tm);
}

void loop() {
  String H, M, S;
  if (RTC.read(tm)) {
    H = alt2digits(tm.Hour);
    M = alt2digits(tm.Minute);
    S = alt2digits(tm.Second);
   Serial.print(H);  Serial.print(':'); Serial.print(M); Serial.print(':'); Serial.println(S); 
   Serial.print(tmYearToCalendar(tm.Year)); Serial.print('/'); Serial.print(tm.Month); Serial.print('/'); Serial.println(tm.Day); 
   }
  delay(1000);
}
