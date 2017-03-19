
#include "setup.h"

FastCRC16 CRC16;

unsigned long previousMillis = 0; 
const long interval = 3000;  
void setup(){
  
  Serial.begin(2400);
//  pinMode(Led,OUTPUT);
  pinMode(RS485Control,OUTPUT);
//  digitalWrite(RS485Control,RS485Receive);
  digitalWrite(RS485Control,RS485Transmit);
  delay(1);
  FN_Send_Code_04(Voltage);
   delay(2000);
  FN_Send_Code_04(Voltage);
  delay(2000);
}

void loop() {
//  digitalWrite(RS485Control,RS485Receive);
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    FN_Send_Code_04(Voltage);
  }
    FN_serialEvent();
  
//   unsigned long currentMillis = millis();
//
//  if (currentMillis - previousMillis >= interval) {
//    previousMillis = currentMillis;
//
//    if(select>7)select=1;
//    switch (select) {
//    case 1:
//      // do something        
//    FN_Send_Code_04(Voltage);FN_serialEvent();select++;
//      break;
//    case 2: 
//
//    FN_Send_Code_04(Current);FN_serialEvent();select++;
//
//    case 3:
//    
//    FN_Send_Code_04(Frequency);FN_serialEvent();select++;
//
//      break;
//    case 4:
//
//    FN_Send_Code_04(Power_Factor);FN_serialEvent();select++;
//
//      break;
//    case 5:
//    
//    FN_Send_Code_04(Reactive_Power);FN_serialEvent();select++;
//
//      break;
//    case 6:
//
//    FN_Send_Code_04(Apparent_Power);FN_serialEvent();select++;
//
//      break; 
//    case 7:
//    FN_Send_Code_04(Active_Power);FN_serialEvent();select++;
//      break;             
//}
//digitalWrite(RS485Control,RS485Transmit);
//    delay(2);
//  }
    if(data_reciver[0]==0x01){
    data_reciver[0]=0;
    float f;
    uint8_t f1=data_reciver[3];
    uint8_t f2=data_reciver[4];
    uint8_t f3=data_reciver[5];
    uint8_t f4=data_reciver[6];
    uint32_t u;
    u = f1;
    u = (u  << 8) +f2 ;
    u = (u  << 8) + f3;
    u = (u  << 8) + f4; //0xec125f46
    
    f = unpack754_32(u);
    Serial.print("Select:=");Serial.print(select-1);Serial.print("___");Serial.println(f);

         for(int i=0;i<9;i++)
         {
          data_reciver[i]=0;
         }
  }
  
  
//   if(data_reciver[0]==0x01){
//     for(int i=0;i<9;i++)
//     {
//      Serial.write(data_reciver[i]);
//     }
//   }
//  while (Serial.available()) {
//    char inChar =Serial.read();
//    Serial.write(inChar);
//  }
  
//  FN_Send_Code_04(Voltage);
//  delay(5000);
//  FN_Send_Code_04(Current);
//  delay(5000);
//  FN_Send_Code_04(Active_Power);
//  delay(5000);
//  FN_Send_Code_04(Apparent_Power);
//  delay(5000);
//  FN_Send_Code_04(Reactive_Power);
//  delay(5000);
//  FN_Send_Code_04(Power_Factor);
//  delay(5000);
//  FN_Send_Code_04(Frequency);
//  delay(5000);
}
void FN_serialEvent() {
  digitalWrite(RS485Control,RS485Receive);
  delay(100)  ;
  while (Serial.available()) {
    char inChar =Serial.read();
    Serial.write(inChar);
    if(inChar==0x01)
    {
      Serial.println("OK");
      index_number=0;
      FLAG_DATA_START = 1;
    }
    if(FLAG_DATA_START==1 && index_number < 9)
    {
      data_reciver[index_number]=inChar;
      index_number++;
    }
    if(index_number==9){
      FLAG_DATA_START=0;
      index_number=0;
    }
  }
}
float Encoder_Data_Receiver()
{
    if(data_reciver[0]==0x01){
    data_reciver[0]=0;
    float f;
    uint8_t f1=data_reciver[3];
    uint8_t f2=data_reciver[4];
    uint8_t f3=data_reciver[5];
    uint8_t f4=data_reciver[6];
    uint32_t u;
    u = f1;
    u = (u  << 8) +f2 ;
    u = (u  << 8) + f3;
    u = (u  << 8) + f4; //0xec125f46
    
    f = unpack754_32(u);
    Serial.print("Select:=");Serial.print(select-1);Serial.print("___");Serial.println(f);
    return f;
  }
}
