#if defined(ARDUINO) && (ARDUINO >= 100)
#  include "Arduino.h"
#else
# if !defined(IRPRONTO)
#   include "WProgram.h"
# endif
#endif
/*
 * 
 */
#include <stdio.h>
#include <limits.h>
#define pack754_32(f) (pack754((f), 32, 8))
#define pack754_64(f) (pack754((f), 64, 11))
#define unpack754_32(i) (unpack754((i), 32, 8))
#define unpack754_64(i) (unpack754((i), 64, 11))
#include <FastCRC.h>
//======================================
#define Led 13
#define RS485Control 12
#define RS485Transmit    HIGH
#define RS485Receive     LOW
//======================================
uint8_t RIR_SlaveAddress=0x01;
uint8_t RIR_Function=0x04;
// uint8_t RIR_StartAddressHigh;
// uint8_t RIR_StartAddressLow;
uint8_t RIR_NumberPointsHigh=0x00;
uint8_t RIR_NumberPointsLow=0x02;
uint32_t RIR_CRC;
// uint8_t RIR_CRC_Low;
// uint8_t RIR_CRC_High;

//======================================
uint16_t Voltage=0x0000;
// uint8_t Voltage_H=0x00;
// uint8_t Voltage_L=0x00;

uint16_t Current=0x0006;
// uint8_t Current_H=0x00;
// uint8_t Current_L=0x06;

uint16_t Active_Power=0x000C;
// uint8_t Active_Power_H=0x00;
// uint8_t Active_Power_L=0x0C;

uint16_t Apparent_Power=0x0012;
// uint8_t Apparent_Power_H=0x00;
// uint8_t Apparent_Power_L=0x12;

uint16_t Reactive_Power=0x0018;
// uint8_t Reactive_Power_H=0x00;
// uint8_t Reactive_Power_L=0x18;

uint16_t Power_Factor=0x001E;
// uint8_t Power_Factor_H=0x00;
// uint8_t Power_Factor_L=0x1E;

uint16_t Frequency=0x0046;
// uint8_t Frequency_H=0x00;
// uint8_t Frequency_L=0x46;



uint8_t data_send[10];
uint8_t data_reciver[10];
uint32_t crc;
uint32_t CRC;
int index_number=0;
int FLAG_DATA_START=0;
int select=1;
