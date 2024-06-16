#ifndef TELEGRAM_HANDLER_H
#define TELEGRAM_HANDLER_H

#include <FastBot.h>
#include "DataPropertiesEeprom.h"
               
const char* WIFI_SSID;
const char* WIFI_PASS;

const char* BOT_TOKEN;
const char* CHAT_ID;

FastBot bot("1111");

class TelegramHandler {

private:
  DataPropertiesEeprom *ptrDataProperties;
  
  String lastMsg;
  byte depth = 0;

  float *m_ptrTempContuor_2;
  float *m_ptrTempContuor_3;
  float *m_ptrTempContuor_4;
  float *m_ptrTempContuor_5;
  float *m_ptrTempContuor_6;

  float *m_ptrTempOut;
  bool *m_ptrIsPumpOn;
  bool *m_ptrOnSendMessage;
  String ws;
  String wp;
  String bt;
  String ci;

public:

  void setDataPropertiesEeprom(DataPropertiesEeprom *dataPro) {
    ptrDataProperties = dataPro;
  }

  void readFromEeprom() {
    ptrDataProperties->readFromEeprom();
    ws = ptrDataProperties->getSSsidT();
    wp = ptrDataProperties->getSPasswordT();
    bt = ptrDataProperties->getSTokenT();
    ci = ptrDataProperties->getSChatIdT();
    
    WIFI_SSID = ws.c_str();
    WIFI_PASS = wp.c_str();
    BOT_TOKEN = bt.c_str();
    CHAT_ID = ci.c_str();
  }

  void alarmSend(){
    bot.deleteMessage(bot.lastBotMsg());
    bot.sendMessage("Alarm alarm alarm");
    checkStatusSend();
    bot.inlineMenu("Sauna collector", printMainMenu());
  }  

  void restartESP10_00(){

    FB_Time t = bot.getTime(ptrDataProperties->getCorrectX());

    if(t.hour == 10){
      if(millis() > 7200000){
        bot.deleteMessage(bot.lastBotMsg());
        bot.sendMessage("Program reset");
        ESP.restart();
      }
    }

  }

  void checkStatusSend() {
    
    FB_Time t = bot.getTime(ptrDataProperties->getCorrectX());

    String tempSensorStr = "2 pool radiator = " + (String)(*m_ptrTempContuor_2) + " *C \n";
    tempSensorStr += "3 2nd floor = " + (String)(*m_ptrTempContuor_3) + "*C \n";
    tempSensorStr += "4 ventilation = " + (String)(*m_ptrTempContuor_4) + "*C,  ";

    if(*m_ptrIsPumpOn){
      tempSensorStr += "pump on \n";
    } else {
      tempSensorStr += "pump off \n";
    }
     
     tempSensorStr += "5 1st floor west = " + (String)(*m_ptrTempContuor_5) + "*C \n";
     tempSensorStr += "6 2-3 floor west = " + (String)(*m_ptrTempContuor_6) + "*C \n";
     tempSensorStr += "t out = " + (String)(*m_ptrTempOut) + "*C \n";
     tempSensorStr += (String)t.hour + ":" + (String)t.minute + "  " + (String)t.day + "/" + (String)t.month +"/"+ (String)t.year;

    bot.sendMessage(tempSensorStr);    
  }

  void setMultyTempStabil(){
    switch(depth){
      case 2:
      ptrDataProperties->setTempStabil_2(lastMsg.toInt());
      break;
      case 3:
      ptrDataProperties->setTempStabil_3(lastMsg.toInt());
      break;
      case 4:
      ptrDataProperties->setTempStabil_4(lastMsg.toInt());
      break;
      case 5:
      ptrDataProperties->setTempStabil_5(lastMsg.toInt());
      break;
      case 6:
      ptrDataProperties->setTempStabil_6(lastMsg.toInt());
      break;
      default:
      break;
    }
  }

  void setMultyStatus(){
    switch(depth){
      case 2:
      ptrDataProperties->setStatus_2();
      break;
      case 3:
      ptrDataProperties->setStatus_3();
      break;
      case 4:
      ptrDataProperties->setStatus_4();
      break;
      case 5:
      ptrDataProperties->setStatus_5();
      break;
      case 6:
      ptrDataProperties->setStatus_6();
      break;
      default:
      break;
    }
  }

  void setMultyCurve(){
    switch(depth){
      case 2:
      ptrDataProperties->setCurve_2(lastMsg.toInt());
      break;
      case 3:
      ptrDataProperties->setCurve_3(lastMsg.toInt());
      break;
      case 4:
      ptrDataProperties->setCurve_4(lastMsg.toInt());
      break;
      case 5:
      ptrDataProperties->setCurve_5(lastMsg.toInt());
      break;
      case 6:
      ptrDataProperties->setCurve_6(lastMsg.toInt());
      break;
      default:
      break;
    }
  } 

  void newMsg(FB_msg &msg) {

    bot.deleteMessage(bot.lastBotMsg());

    if (msg.data == "Check status") {
      checkStatusSend();
      depth = 0;
    }

    if (msg.data == "2 pool radiator") {      
      depth = 2;
    }

    if (msg.data == "3 2nd floor") {      
      depth = 3;
    }

    if (msg.data == "4 ventilation") {      
      depth = 4;
    }

    if (msg.data == "5 1st floor west") {      
      depth = 5;
    }

    if (msg.data == "6 2-3 floor west") {      
      depth = 6;
    }
    
    if (msg.data == "out sensor") {      
      depth = 20;
    }
    
    if (msg.data == "Notice") {
      *m_ptrOnSendMessage = !(*m_ptrOnSendMessage);
      ptrDataProperties->saveToEeprom();
      depth = 0;
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (msg.data == "manual mode *t") {
    setMultyTempStabil();
  }

  if (msg.data == "manual mode status") {
    setMultyStatus();
  }

  if (msg.data == "temperature curve") {
    setMultyCurve();
  }


  if (msg.data == "start pump time") {
    ptrDataProperties->setStartTime(lastMsg.toInt());
  }

  if (msg.data == "end pump time") {
    ptrDataProperties->setEndTime(lastMsg.toInt());
  }

  if (msg.data == "time zone") {
    ptrDataProperties->setCorrectX(lastMsg.toInt());
  }

  if (msg.data == "parametr A") {
    ptrDataProperties->setCorrectA(lastMsg.toInt());
  }
  
  if (msg.data == "out lower") {
    ptrDataProperties->setOutLower(lastMsg.toInt());
  }

  if (msg.data == "contour lower") {
    ptrDataProperties->setContuorLower(lastMsg.toInt());
  }

  if (msg.data == "number contour") {
    ptrDataProperties->setNumberContuor(lastMsg.toInt());
  }

  if (msg.data == "count message") {
    ptrDataProperties->setCountMessage(lastMsg.toInt());
  }
  
  if (msg.data == "radiator pool") {
    //Serial.println("if (msg.data == radiator pool) {");
    ptrDataProperties->changeOnRadiatorPool();
  }

  if (msg.data == "type servo 2") {
    ptrDataProperties->changeTypeServoNO_2();
  }
  if (msg.data == "type servo 3") {
    ptrDataProperties->changeTypeServoNO_3();
  }
  if (msg.data == "type servo 4") {
    ptrDataProperties->changeTypeServoNO_4();
  }
  if (msg.data == "type servo 5") {
    ptrDataProperties->changeTypeServoNO_5();
  }
  if (msg.data == "type servo 6") {
    ptrDataProperties->changeTypeServoNO_6();
  }

    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (msg.data == "save") {
      ptrDataProperties->saveToEeprom();
    }

    if (msg.data == "back") {      
      depth = 0;
    }

    if(depth == 0){    
      bot.inlineMenu("Sauna collector", printMainMenu());
    }
    
    if(depth == 2){    
      bot.inlineMenu("2 pool radiator", printMenu2(ptrDataProperties -> getTempStabil_2(), ptrDataProperties -> getStatus_2(), ptrDataProperties -> getCurve_2()));
    }

    if(depth == 3){    
      bot.inlineMenu("3 2nd floor3", printMenuAll(ptrDataProperties -> getTempStabil_3(), ptrDataProperties -> getStatus_3(), ptrDataProperties -> getCurve_3()));
    }

    if(depth == 4){    
      bot.inlineMenu("4 ventilation", printMenu4(ptrDataProperties -> getTempStabil_4(), ptrDataProperties -> getStatus_4(), ptrDataProperties -> getCurve_4(), ptrDataProperties -> getStartTime(), ptrDataProperties -> getEndTime()));
    }

    if(depth == 5){    
      bot.inlineMenu("5 1st floor west", printMenuAll(ptrDataProperties -> getTempStabil_5(), ptrDataProperties -> getStatus_5(), ptrDataProperties -> getCurve_5()));
    }

    if(depth == 6){    
      bot.inlineMenu("6 2-3 floor west", printMenuAll(ptrDataProperties -> getTempStabil_6(), ptrDataProperties -> getStatus_6(), ptrDataProperties -> getCurve_6()));
    }

    if(depth == 20){    
      
      bot.inlineMenu("out sensor", printMenuOut(ptrDataProperties -> getCorrectX(), ptrDataProperties -> getCorrectA(), 
        ptrDataProperties -> getOutLower(), ptrDataProperties -> getContuorLower(), ptrDataProperties -> getNumberContuor(), 
        ptrDataProperties -> getCountMessage()));
    }

    lastMsg = msg.text;
  }



   void changeStateSend(bool withoutCondition) {
  //   // if (*m_ptrOnSendMessage || withoutCondition) {
  //   //   bot.deleteMessage(bot.lastBotMsg());
  //   //   bot.inlineMenu("2 pool radiator", printMenu2(ptrDataProperties -> getTempStabil_2(), ptrDataProperties -> getStatus_2(), ptrDataProperties -> getCurve_2()));
  //   // }
   }

  String printMainMenu(){    

    String menu1;
      if (*m_ptrOnSendMessage) {
        menu1 = F("Check status \n 2 pool radiator \n 3 2nd floor \n 4 ventilation \n 5 1st floor west \n 6 2-3 floor west \n out sensor \n Notice \t On");
      } else {
        menu1 = F("Check status \n 2 pool radiator \n 3 2nd floor \n 4 ventilation \n 5 1st floor west \n 6 2-3 floor west \n out sensor \n Notice \t Off");
      }
    return menu1;
  }

  String printMenu4(int temp, bool status, int curve, int startTime, int endTime){

    String menu1;
    menu1 = "manual mode *t \t" + (String)temp +" \n manual mode status \t" + getBoolString(status) + "\n temperature curve \t" + (String)curve + "\n start pump time \t" + (String)startTime + "\n end pump time \t" + (String)endTime +"\n save \n back";
    return menu1;
  }

  String printMenu2(int temp, bool status, int curve){
    String menu1;
    menu1 = "radiator pool \t" + getBoolString(ptrDataProperties->getOnRadiatorPool()) + "\n manual mode *t \t" + (String)temp +" \n manual mode status \t" + getBoolString(status) + "\n temperature curve \t" + (String)curve + "\n save \n back";
    return menu1;
  }

  String printMenuAll(int temp, bool status, int curve){
    String menu1;
    menu1 = "manual mode *t \t" + (String)temp +" \n manual mode status \t" + getBoolString(status) + "\n temperature curve \t" + (String)curve + "\n save \n back";
    return menu1;
  }
  
  String printMenuOut(int x, int a, int outLower, int contourLower, int numberContourLower, int counMessage){
    String menu1;
    menu1 = "time zone \t" + (String)x + "\n parametr A \t" + (String)a 
    + "\n out lower \t" + (String)outLower + "\n contour lower \t" + (String)contourLower + "\n number contour \t" + (String)numberContourLower + "\n count message \t" + (String)counMessage 
    + "\n type servo 2 \t" + getNOorNCtype(ptrDataProperties->getTypeServoNO_2()) 
    + "\n type servo 3 \t" + getNOorNCtype(ptrDataProperties->getTypeServoNO_3())
    + "\n type servo 4 \t" + getNOorNCtype(ptrDataProperties->getTypeServoNO_4())
    + "\n type servo 5 \t" + getNOorNCtype(ptrDataProperties->getTypeServoNO_5())
    + "\n type servo 6 \t" + getNOorNCtype(ptrDataProperties->getTypeServoNO_6())
    + "\n save \n back";
    return menu1;
  }  

  String getBoolString(bool status){
    if(status) {return "on";}
    return "off";
  }

  String getNOorNCtype(bool status){
    if(status) {return "NO";}
    return "NC";
  }

  void setPtrTempContuor_2(float *ptr){
    m_ptrTempContuor_2 = ptr;
  }
  void setPtrTempContuor_3(float *ptr){
    m_ptrTempContuor_3 = ptr;
  }
  void setPtrTempContuor_4(float *ptr){
    m_ptrTempContuor_4 = ptr;
  }
  void setPtrTempContuor_5(float *ptr){
    m_ptrTempContuor_5 = ptr;
  }
  void setPtrTempContuor_6(float *ptr){
    m_ptrTempContuor_6 = ptr;
  }

  void setPtrTempOut(float *ptr){
    m_ptrTempOut = ptr;
  }  

  void setIsPumpOn(bool *ptr){
    m_ptrIsPumpOn = ptr;
  }
  void setSendMessage(bool *ptr){
    m_ptrOnSendMessage = ptr;
  }  

  // void setTelegramSsidT(String name) {
  //   WIFI_SSID = name.c_str();
  // }
  // void setTelegramPasswordT(String name) {
  //   WIFI_PASS = name.c_str();
  // }
  // void setTelegramTokenT(String name) {
  //   //BOT_TOKEN = name.c_str();
  // }
  // void setTelegramChatIdT(String name) {
  //   //CHAT_ID = name.c_str();
  // }


};

#endif
