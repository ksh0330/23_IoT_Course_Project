//RFID
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN); //rfid 객체 생성

//7세그먼트
int datapin = 4;
int latchpin = 5;
int clockpin = 6; 

//RFID 출입 LED
int RLED=A0;
int GLED=A1;
int Door = 8;

//Tact Switch
int sw1 = 2;
int sw2 = 3;

//7 Segment 값 조정(인원)
int u1 = 0;
int u2 = 0;
int sum = 0;

void setup() 
{
  pinMode(RLED, OUTPUT);      // R LED 출력
  pinMode(GLED, OUTPUT);      // G LED 출력
  pinMode(Door, OUTPUT);

  pinMode(sw1, INPUT);        // sw1 입력
  pinMode(sw2, INPUT);        // sw2 입력

  pinMode(datapin, OUTPUT);   //datapin 출력
  pinMode(latchpin, OUTPUT);  //latchpin 출력
  pinMode(clockpin, OUTPUT);  //clockpin 출력

  Serial.begin(9600);         // 시리얼 통신 시작
  SPI.begin();                // SPI 통신 시작
  rfid.PCD_Init();            // rfid(MFRC522) 초기화
  Serial.println("===RFID START===");
  Serial.println();

  digitalWrite(latchpin, LOW);
  shiftOut(datapin, clockpin, LSBFIRST, 252);
  digitalWrite(latchpin, HIGH);
}

void loop() 
{
  int a[10] = {252, 96, 218, 242, 102, 182, 190, 224, 254, 246};

  //Tact Swich
  if (digitalRead(sw1) == LOW || (digitalRead(sw2) == LOW)){
    
    Serial.println("||Door Open||");
    digitalWrite(GLED,HIGH); 
    digitalWrite(Door,HIGH);
    delay(1000);
    Serial.println("||Door Close||");
    digitalWrite(GLED,LOW);
    digitalWrite(Door,LOW);
    Serial.print("현재 내부 인원 : "); 
    Serial.println(sum);
    Serial.println("========================================================================================");
  }

  //RFID
  // 새카드 접촉이 있을 때만 다음 단계로 넘어감
  if ( ! rfid.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // 카드 읽힘이 제대로 되면 다음으로 넘어감
  if ( ! rfid.PICC_ReadCardSerial())
  {
    return;
  }
  // UID 값을 16진 값으로 읽고 저장한 후 시리얼 모니터로 표시
  Serial.print("사용자명 : ");
  String content= "";
  byte letter;
  for (byte i = 0; i < rfid.uid.size; i++)
  {
     content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(rfid.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  
  //사용자 ID 추가, 7 Segment 값 조정
  if (content.substring(1) == "7B 90 08 1C"){
    Serial.println("김철수");
    if (u1 / 1 == 0){
      u1++;
    }
    else{
      u1--;
    }
  }
  if (content.substring(1) == "2A C5 BA 15"){
    Serial.println("김영희");
    if (u2 / 1 == 0){
      u2++;
    }
    else{
      u2--;
    }
  }


  //Message, 내부인원, 7 Segment 값 종합
  if (content.substring(1) == "7B 90 08 1C" || (content.substring(1) == "2A C5 BA 15"))// 엑세스 승인 하고자하는 UID 기록
  {
    sum = u1 + u2;

    digitalWrite(latchpin, LOW);
    shiftOut(datapin, clockpin, LSBFIRST, a[sum]);
    digitalWrite(latchpin, HIGH);

    Serial.print("Message : ");
    Serial.println("승인된 사용자입니다.");
    
    Serial.print("현재 내부 인원 : ");
    Serial.println(sum);
    Serial.println("========================================================================================");
    digitalWrite(GLED,HIGH);
    digitalWrite(Door,HIGH); 
    delay(1000); 
    digitalWrite(GLED,LOW); // 시간지연 후 LED 끈다
    digitalWrite(Door,LOW);        
  }
  else   {    // 서보모터는 작동 없이 Red LED만 켜고 끈다
    Serial.println();
    Serial.print("Message : ");
    Serial.println("미승인된 사용자입니다."); // 그외 UID는 승인 거부 표시
    Serial.print("현재 내부 인원 : ");
    Serial.println(sum);
    Serial.println("========================================================================================");
    digitalWrite(RLED,HIGH); 
    delay(1000);
    digitalWrite(RLED,LOW);
  }

}
