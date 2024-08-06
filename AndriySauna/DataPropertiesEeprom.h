#ifndef DATA_PROPERTIES_EEPROM_H
#define DATA_PROPERTIES_EEPROM_H

#include <EEPROM.h>
#define LED_POOL_RADIATOR_ON 2

class DataPropertiesEeprom{

    private:
    int m_tStabil_2;
    int m_tStabil_3;
    int m_tStabil_4;
    int m_tStabil_5;
    int m_tStabil_6;

    bool m_status_2;
    bool m_status_3;
    bool m_status_4;
    bool m_status_5;
    bool m_status_6;
    
    int m_cufve_2;
    int m_cufve_3;
    int m_cufve_4;
    int m_cufve_5;
    int m_cufve_6;

    int m_startTime;
    int m_endTime;

    int m_correctX;
    int m_correctA;

    int m_outLower;
    int m_contuorLower;
    int m_numberContuor;
    int m_countMessage;
    bool *m_ptrOnSendMessage;

    bool m_onRadiatiorPool;
    bool m_typeServoNO_2;
    bool m_typeServoNO_3;
    bool m_typeServoNO_4;
    bool m_typeServoNO_5;
    bool m_typeServoNO_6;

    unsigned long nextTouch;

    String m_ssid;
    String m_password;
    String m_token;
    String m_chatId;


public: 
    
void saveToEeprom(){  

  EEPROM.write(0, m_tStabil_2 + 140);
  EEPROM.write(1, m_tStabil_3 + 140);
  EEPROM.write(2, m_tStabil_4 + 140);
  EEPROM.write(3, m_tStabil_5 + 140);
  EEPROM.write(4, m_tStabil_6 + 140);

  EEPROM.write(5, m_status_2);
  EEPROM.write(6, m_status_3);
  EEPROM.write(7, m_status_4);
  EEPROM.write(8, m_status_5);
  EEPROM.write(9, m_status_6);
  
  EEPROM.write(10, m_cufve_2);
  EEPROM.write(11, m_cufve_3);
  EEPROM.write(12, m_cufve_4);
  EEPROM.write(13, m_cufve_5);
  EEPROM.write(14, m_cufve_6);

  EEPROM.write(15, m_startTime);
  EEPROM.write(16, m_endTime);

  EEPROM.write(17, m_correctX);
  EEPROM.write(18, m_correctA + 140);
  
  EEPROM.write(19, m_outLower + 140);
  EEPROM.write(20, m_contuorLower + 140);
  EEPROM.write(21, m_numberContuor);
  EEPROM.write(22, m_countMessage);
  EEPROM.write(23, *m_ptrOnSendMessage);

  EEPROM.write(24, m_onRadiatiorPool);
  EEPROM.write(25, m_typeServoNO_2);
  EEPROM.write(26, m_typeServoNO_3);
  EEPROM.write(27, m_typeServoNO_4);
  EEPROM.write(28, m_typeServoNO_5);
  EEPROM.write(29, m_typeServoNO_6);

  for(int i = 0; i < m_ssid.length(); i++){
    EEPROM.write(30 + i, m_ssid[i]);
  }
  EEPROM.write(70, m_ssid.length());

  for(int i = 0; i < m_password.length(); i++){
    EEPROM.write(71 + i, m_password[i]);
  }
  EEPROM.write(100, m_password.length());

  for(int i = 0; i < m_token.length(); i++){
    EEPROM.write(101 + i, m_token[i]);
  }
  EEPROM.write(160, m_token.length());

  for(int i = 0; i < m_chatId.length(); i++){
    EEPROM.write(161 + i, m_chatId[i]);
  }
  EEPROM.write(210, m_chatId.length());

  EEPROM.commit(); 
  Serial.println("EEPROM.commit(); ");
  
}


void readFromEeprom(){

  m_tStabil_2 = EEPROM.read(0) - 140;
  m_tStabil_3 = EEPROM.read(1) - 140;
  m_tStabil_4 = EEPROM.read(2) - 140;
  m_tStabil_5 = EEPROM.read(3) - 140;
  m_tStabil_6 = EEPROM.read(4) - 140;

  m_status_2 = EEPROM.read(5);
  m_status_3 = EEPROM.read(6);
  m_status_4 = EEPROM.read(7);
  m_status_5 = EEPROM.read(8);
  m_status_6 = EEPROM.read(9);

  m_cufve_2 = EEPROM.read(10);
  m_cufve_3 = EEPROM.read(11);
  m_cufve_4 = EEPROM.read(12);
  m_cufve_5 = EEPROM.read(13);
  m_cufve_6 = EEPROM.read(14);

  m_startTime = EEPROM.read(15);
  m_endTime = EEPROM.read(16);

  m_correctX = EEPROM.read(17);
  m_correctA = EEPROM.read(18) - 140;

  m_outLower = EEPROM.read(19) - 140;
  m_contuorLower = EEPROM.read(20) - 140;
  m_numberContuor = EEPROM.read(21);
  m_countMessage = EEPROM.read(22);
  *m_ptrOnSendMessage = EEPROM.read(23);

  m_onRadiatiorPool = EEPROM.read(24);
  m_typeServoNO_2 = EEPROM.read(25);
  m_typeServoNO_3 = EEPROM.read(26);
  m_typeServoNO_4 = EEPROM.read(27);
  m_typeServoNO_5 = EEPROM.read(28);
  m_typeServoNO_6 = EEPROM.read(29);

  m_ssid = "";
  for(int i = 0; i < EEPROM.read(70); i++){    
    if(EEPROM.read(30 + i) > 31 && EEPROM.read(30 + i) < 127){
      m_ssid += char(EEPROM.read(30 + i));
    }
  }

  m_password = "";
  for(int i = 0; i < EEPROM.read(100); i++){    
    if(EEPROM.read(71 + i) > 31 && EEPROM.read(71 + i) < 127){
      m_password += char(EEPROM.read(71 + i));
    }
  }

  m_token = "";
  for(int i = 0; i < EEPROM.read(160); i++){
    if(EEPROM.read(101 + i) > 31 && EEPROM.read(101 + i) < 127){
      m_token += char(EEPROM.read(101 + i));
    }
  }

  m_chatId = "";
  for(int i = 0; i < EEPROM.read(210); i++){
    if(EEPROM.read(161 + i) > 31 && EEPROM.read(161 + i) < 127){
      m_chatId += char(EEPROM.read(161 + i));
    }
  }  

  if(m_onRadiatiorPool){
    digitalWrite(LED_POOL_RADIATOR_ON, LOW);
  } else {
    digitalWrite(LED_POOL_RADIATOR_ON, HIGH);
  }
  
}

void setTempStabil_2(int temp){
  m_tStabil_2 = temp;
}
void setTempStabil_3(int temp){
  m_tStabil_3 = temp;
}
void setTempStabil_4(int temp){
  m_tStabil_4 = temp;
}
void setTempStabil_5(int temp){
  m_tStabil_5 = temp;
}
void setTempStabil_6(int temp){
  m_tStabil_6 = temp;
}

void setStatus_2(){
  m_status_2 = !m_status_2;
}
void setStatus_3(){
  m_status_3 = !m_status_3;
}
void setStatus_4(){
  m_status_4 = !m_status_4;
}
void setStatus_5(){
  m_status_5 = !m_status_5;
}
void setStatus_6(){
  m_status_6 = !m_status_6;
}

void setStatus_2(bool state){
  m_status_2 = state;
}
void setStatus_3(bool state){
  m_status_3 = state;
}
void setStatus_4(bool state){
  m_status_4 = state;
}
void setStatus_5(bool state){
  m_status_5 = state;
}
void setStatus_6(bool state){
  m_status_6 = state;
}
//---------------------------------------

void changeOnRadiatorPool(bool state){

Serial.println("void changeOnRadiatorPool(){");
  if(millis() - nextTouch > 10000){
    m_onRadiatiorPool = state;
    nextTouch = millis();
  }
  if(m_onRadiatiorPool){
    digitalWrite(LED_POOL_RADIATOR_ON, LOW);
  } else {
    digitalWrite(LED_POOL_RADIATOR_ON, HIGH);
  }
  saveToEeprom();
}

void changeOnRadiatorPool(){

Serial.println("void changeOnRadiatorPool(){");
  if(millis() - nextTouch > 10000){
    m_onRadiatiorPool = !m_onRadiatiorPool;
    nextTouch = millis();
  }
  if(m_onRadiatiorPool){
    digitalWrite(LED_POOL_RADIATOR_ON, LOW);
  } else {
    digitalWrite(LED_POOL_RADIATOR_ON, HIGH);
  }
  /////////////saveToEeprom();
}

void changeTypeServoNO_2(){
  m_typeServoNO_2 = !m_typeServoNO_2;
}
void changeTypeServoNO_3(){
  m_typeServoNO_3 = !m_typeServoNO_3;
}
void changeTypeServoNO_4(){
  m_typeServoNO_4 = !m_typeServoNO_4;
}
void changeTypeServoNO_5(){
  m_typeServoNO_5 = !m_typeServoNO_5;
}
void changeTypeServoNO_6(){
  m_typeServoNO_6 = !m_typeServoNO_6;
}

//------------------------------------------
void setCurve_2(int value){
  m_cufve_2 = value;
}
void setCurve_3(int value){
  m_cufve_3 = value;
}
void setCurve_4(int value){
  m_cufve_4 = value;
}
void setCurve_5(int value){
  m_cufve_5 = value;
}
void setCurve_6(int value){
  m_cufve_6 = value;
}

void setStartTime(int value){
  m_startTime = value;
}
void setEndTime(int value){
  m_endTime = value;
}

void setCorrectX(int value){
  m_correctX = value;
}
void setCorrectA(int value){
  m_correctA = value;
}

void setOutLower(int temp){
  m_outLower = temp;
}
void setContuorLower(int temp){
  m_contuorLower = temp;
}
void setNumberContuor(int value){
  m_numberContuor = value;
}
void setCountMessage(int value){
  m_countMessage = value;
}

void setPtrOnSendMessage(bool *ptr){
  m_ptrOnSendMessage = ptr;
}

void setOnRadiatorPool(bool value){
  m_onRadiatiorPool = value;
}

void setTypeServoNO_2(bool value){
  m_typeServoNO_2 = value;
}
void setTypeServoNO_3(bool value){
  m_typeServoNO_3 = value;
}
void setTypeServoNO_4(bool value){
  m_typeServoNO_4 = value;
}
void setTypeServoNO_5(bool value){
  m_typeServoNO_5 = value;
}
void setTypeServoNO_6(bool value){
  m_typeServoNO_6 = value;
}

void setSsidT(String name){
  m_ssid = name;
}
void setPasswordT(String name){
  m_password = name;
}
void setTokenT(String name){
  m_token = name;
}
void setChatIdT(String name){
  m_chatId = name;
}

int getTempStabil_2(){return m_tStabil_2;}
int getTempStabil_3(){return m_tStabil_3;}
int getTempStabil_4(){return m_tStabil_4;}
int getTempStabil_5(){return m_tStabil_5;}
int getTempStabil_6(){return m_tStabil_6;}

bool getStatus_2(){return m_status_2;}
bool getStatus_3(){return m_status_3;}
bool getStatus_4(){return m_status_4;}
bool getStatus_5(){return m_status_5;}
bool getStatus_6(){return m_status_6;}

int getCurve_2(){return m_cufve_2;}
int getCurve_3(){return m_cufve_3;}
int getCurve_4(){return m_cufve_4;}
int getCurve_5(){return m_cufve_5;}
int getCurve_6(){return m_cufve_6;}

int getStartTime(){return m_startTime;}
int getEndTime(){return m_endTime;}

int getCorrectX(){return m_correctX;}
int getCorrectA(){return m_correctA;}

int getOutLower(){return m_outLower;}
int getContuorLower(){return m_contuorLower;}
int getNumberContuor(){return m_numberContuor;}
int getCountMessage(){return m_countMessage;}

bool getOnRadiatorPool(){return m_onRadiatiorPool;}
bool getTypeServoNO_2(){return m_typeServoNO_2;}
bool getTypeServoNO_3(){return m_typeServoNO_3;}
bool getTypeServoNO_4(){return m_typeServoNO_4;}
bool getTypeServoNO_5(){return m_typeServoNO_5;}
bool getTypeServoNO_6(){return m_typeServoNO_6;}

String getSTempStabil_2(){return (String)m_tStabil_2;}
String getSTempStabil_3(){return (String)m_tStabil_3;}
String getSTempStabil_4(){return (String)m_tStabil_4;}
String getSTempStabil_5(){return (String)m_tStabil_5;}
String getSTempStabil_6(){return (String)m_tStabil_6;}

String boolToString(bool state){
  if(state){
    return "on";    
  } else {
    return "off";
  }
}

String getSStatus_2(){
  return boolToString(boolToString(m_status_2));
}
String getSStatus_3(){
  return boolToString(boolToString(m_status_3));
}
String getSStatus_4(){
  return boolToString(boolToString(m_status_4));
}
String getSStatus_5(){
  return boolToString(boolToString(m_status_5));
}
String getSStatus_6(){
  return boolToString(boolToString(m_status_6));
}

String getSCurve_2(){return (String)m_cufve_2;}
String getSCurve_3(){return (String)m_cufve_3;}
String getSCurve_4(){return (String)m_cufve_4;}
String getSCurve_5(){return (String)m_cufve_5;}
String getSCurve_6(){return (String)m_cufve_6;}

String getSStartTime(){return (String)m_startTime;}
String getSEndTime(){return (String)m_endTime;}

String getSCorrectX(){return (String)m_correctX;}
String getSCorrectA(){return (String)m_correctA;}

String getSOutLower(){return (String)m_outLower;}
String getSContuorLower(){return (String)m_contuorLower;}
String getSNumberContuor(){return (String)m_numberContuor;}
String getSCountMessage(){return (String)m_countMessage;}

String getSSsidT(){return m_ssid;}
String getSPasswordT(){return m_password;}
String getSTokenT(){return m_token;}
String getSChatIdT(){return m_chatId;}

};

#endif
