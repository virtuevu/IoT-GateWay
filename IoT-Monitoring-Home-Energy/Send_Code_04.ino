void FN_Send_Code_04(uint16_t Parameter)
{
    digitalWrite(12,HIGH);
    delay(200);
    data_send[0]=RIR_SlaveAddress; // 01
    data_send[1]=RIR_Function;// 04
    data_send[2]=((Parameter>>8) & 0xff);
    data_send[3]=(Parameter & 0x00ff);
    data_send[4]=RIR_NumberPointsHigh; //00
    data_send[5]=RIR_NumberPointsLow;// 02

    RIR_CRC=CRC16.modbus(data_send,6);
    
    data_send[6]=(RIR_CRC & 0x00ff); // low
    data_send[7]=((RIR_CRC>>8) & 0xff); // high

    Serial.write(data_send[0]);
    Serial.write(data_send[1]);
    Serial.write(data_send[2]);
    Serial.write(data_send[3]);
    Serial.write(data_send[4]);
    Serial.write(data_send[5]);
    Serial.write(data_send[6]);
    Serial.write(data_send[7]);
    
}

