#include <stdio.h>
#include <limits.h>

#define pack754_32(f) (pack754((f), 32, 8))
#define pack754_64(f) (pack754((f), 64, 11))
#define unpack754_32(i) (unpack754((i), 32, 8))
#define unpack754_64(i) (unpack754((i), 64, 11))

#include <FastCRC.h>

#include "setup.h"

FastCRC16 CRC16;

unsigned long previousMillis = 0; 
const long interval = 3000;  
void setup(){
  
  Serial.begin(2400);
  pinMode(Led,OUTPUT);
  pinMode(RS485Control,OUTPUT);
//  digitalWrite(RS485Control,RS485Receive);
  digitalWrite(12,HIGH);
  delay(2000);
  FN_Send_Code_04(Voltage);
   delay(2000);
  FN_Send_Code_04(Voltage);
   delay(2000);
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

long double unpack754(uint64_t i, unsigned bits, unsigned expbits)
{
  long double result;
  long long shift;
  unsigned bias;
  unsigned significandbits = bits - expbits - 1; // -1 for sign bit

  if (i == 0) return 0.0;

  // pull the significand
  result = (i&((1LL<<significandbits)-1)); // mask
  result /= (1LL<<significandbits); // convert back to float
  result += 1.0f; // add the one back on

  // deal with the exponent
  bias = (1<<(expbits-1)) - 1;
  shift = ((i>>significandbits)&((1LL<<expbits)-1)) - bias;
  while(shift > 0) { result *= 2.0; shift--; }
  while(shift < 0) { result /= 2.0; shift++; }

  // sign it
  result *= (i>>(bits-1))&1? -1.0: 1.0;

  return result;
}
uint64_t pack754(long double f, unsigned bits, unsigned expbits)
{
  long double fnorm;
  int shift;
  long long sign, exp, significand;
  unsigned significandbits = bits - expbits - 1; // -1 for sign bit

  if (f == 0.0) return 0; // get this special case out of the way

  // check sign and begin normalization
  if (f < 0) { sign = 1; fnorm = -f; }
  else { sign = 0; fnorm = f; }

  // get the normalized form of f and track the exponent
  shift = 0;
  while(fnorm >= 2.0) { fnorm /= 2.0; shift++; }
  while(fnorm < 1.0) { fnorm *= 2.0; shift--; }
  fnorm = fnorm - 1.0;

  // calculate the binary form (non-float) of the significand data
  significand = fnorm * ((1LL<<significandbits) + 0.5f);

  // get the biased exponent
  exp = shift + ((1<<(expbits-1)) - 1); // shift + bias

  // return the final answer
  return (sign<<(bits-1)) | (exp<<(bits-expbits-1)) | significand;
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
