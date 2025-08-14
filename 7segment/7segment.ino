int datapin = 4;
int latchpin = 5;  
int clockpin = 6;  

void setup(){
 Serial.begin(9600);
 pinMode(datapin, OUTPUT);
 pinMode(latchpin, OUTPUT);
 pinMode(clockpin, OUTPUT);
}

void loop(){
 // 숫자를 0~9까지 반복
 int a[10] = {252, 96, 218, 242, 102, 182, 190, 224, 254, 246};

  
 for (int i = 0; i < 10; i++){
   digitalWrite(latchpin, LOW);
   shiftOut(datapin, clockpin, LSBFIRST, a[i]);
   digitalWrite(latchpin, HIGH);
   
   Serial.println(i);
   delay(1000);
   
 }
}