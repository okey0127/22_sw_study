#define R_pin 10
#define G_pin 11
#define B_pin 12

int R = 255;
int G = 255;
int B = 255;

int nState = 1;
void setup() {
  Serial.begin(9600);
  pinMode(R_pin, OUTPUT);
  pinMode(G_pin, OUTPUT);
  pinMode(B_pin, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  

  if(nState){
    Serial.println("Enter R, G ,B");
    nState = 0;
    }
  while(Serial.available()==0){} // wait for input
  int R = Serial.parseInt(); // 정수로 표현된 문자를 뽑아 정수로 변환 후 반환
  Serial.println(R);
  while(Serial.available()==0){}
  int G = Serial.parseInt();
  Serial.println(G);
  while(Serial.available()==0){}
  int B = Serial.parseInt();
  Serial.println(B);
  Serial.print(R); Serial.print(" "); Serial.print(G);Serial.print(" "); Serial.println(B);
  
  nState = 1;
  

  analogWrite(R_pin, R);
  analogWrite(G_pin, G);
  analogWrite(B_pin, B);
  
  int a = Serial.read();
  Serial.println(" ");
}
