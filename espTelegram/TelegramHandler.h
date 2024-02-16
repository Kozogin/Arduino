#ifndef TELEGRAM_HANDLER_H
#define TELEGRAM_HANDLER_H

#include <FastBot.h>
#include "DataPropertiesEeprom.h"

#define WIFI_SSID "ssid"
#define WIFI_PASS "password"

#define BOT_TOKEN "telegram bot token"
#define CHAT_ID "chat id"

FastBot bot(BOT_TOKEN);

class TelegramHandler {

private:

  DataPropertiesEeprom *ptrDataProperties;

  bool isMenuProperties;
  bool isMenuProperties_2;
  String lastMsg;
  byte depth = 0;
  bool noticeOnOff;

  int *tempSensor;
  int *tempSensor_2;

  bool *ptrIsPumpWork;
  bool *ptrIsPumpWork_2;

public:

  void setDataPropertiesEeprom(DataPropertiesEeprom *dataPro) {
    ptrDataProperties = dataPro;
  }

  void readFromEeprom() {
    ptrDataProperties->readFromEeprom();
  }

  void checkStatusSend() {

    String tempSensorStr = "temp = ";
    if (*ptrIsPumpWork) {
      tempSensorStr += (String)(*tempSensor) + "*C pump крутить, \n temp 2 = ";
    } else {
      tempSensorStr += (String)(*tempSensor) + "*C pump зупинена, \n temp 2 = ";
    }
    if (*ptrIsPumpWork_2) {
      tempSensorStr += (String)(*tempSensor_2) + "*C pump rotate, ";
    } else {
      tempSensorStr += (String)(*tempSensor_2) + "*C pump 2 stopped, ";
    }
    bot.sendMessage(tempSensorStr);
  }



  void newMsg(FB_msg &msg) {

    Serial.println(msg.toString());

    bot.deleteMessage(bot.lastBotMsg());

    if (msg.data == "Check status") {

      checkStatusSend();

      depth = 0;
      isMenuProperties = false;
      isMenuProperties_2 = false;
    }

    if (msg.data == "Properties") {
      isMenuProperties = true;
      isMenuProperties_2 = false;
      depth = 1;
    }
    if (msg.data == "Properties 2") {
      isMenuProperties = false;
      isMenuProperties_2 = true;
      depth = 1;
    }

    if (msg.data == "Notice change On Off") {
      noticeOnOff = !noticeOnOff;
      isMenuProperties = false;
      isMenuProperties_2 = false;
      depth = 0;
    }


    if (msg.data == "temp pump ON SET") {
      ptrDataProperties->setTempPumpON(lastMsg.toInt());
      depth = 1;
    }
    if (msg.data == "temp pump ON SET 2") {
      ptrDataProperties->setTempPumpON_2(lastMsg.toInt());
      depth = 1;
    }

    if (msg.data == "temp pump OFF SET") {
      ptrDataProperties->setTempPumpOFF(lastMsg.toInt());
      depth = 1;
    }
    if (msg.data == "temp pump OFF SET 2") {
      ptrDataProperties->setTempPumpOFF_2(lastMsg.toInt());
      depth = 1;
    }

    if (msg.data == "time pump ON SET") {
      ptrDataProperties->setTimePumpON(lastMsg.toInt());
      depth = 1;
    }
    if (msg.data == "time pump ON SET 2") {
      ptrDataProperties->setTimePumpON_2(lastMsg.toInt());
      depth = 1;
    }

    if (msg.data == "temp min SET") {
      ptrDataProperties->setTempMin(lastMsg.toInt());
      depth = 1;
    }
    if (msg.data == "temp min SET 2") {
      ptrDataProperties->setTempMin_2(lastMsg.toInt());
      depth = 1;
    }

    if (msg.data == "temp max SET") {
      ptrDataProperties->setTempMax(lastMsg.toInt());
      depth = 1;
    }
    if (msg.data == "temp max SET 2") {
      ptrDataProperties->setTempMax(lastMsg.toInt());
      depth = 1;
    }

    if (msg.data == "Save") {
      ptrDataProperties->saveToEeprom();
      depth = 1;
    }

    if (msg.data == "Back" && depth == 1) {

      isMenuProperties = false;
      isMenuProperties_2 = false;
      depth = 0;
    }

    if (isMenuProperties) {
      String properties = "temp pump ON SET \t" + ptrDataProperties->getSTempPumpON() + "*C \n"
                          + "temp pump OFF SET \t" + ptrDataProperties->getSTempPumpOFF() + "*C \n"
                          + "time pump ON SET \t" + ptrDataProperties->getSTimePumpON() + " min \n"
                          + "temp min SET\t" + ptrDataProperties->getSTempMin() + " *C \n "
                          + "temp max SET\t" + ptrDataProperties->getSTempMax() + " *C \n Save \n Back";

      bot.inlineMenu("Properties", properties);
      depth = 1;
    }

    if (isMenuProperties_2) {
      String properties = "temp pump ON SET 2 \t" + ptrDataProperties->getSTempPumpON_2() + "*C \n"
                          + "temp pump OFF SET 2 \t" + ptrDataProperties->getSTempPumpOFF_2() + "*C \n"
                          + "time pump ON SET 2 \t" + ptrDataProperties->getSTimePumpON_2() + " min \n"
                          + "temp min SET 2 \t" + ptrDataProperties->getSTempMin_2() + " *C \n "
                          + "temp max SET 2 \t" + ptrDataProperties->getSTempMax_2() + " *C \n Save \n Back";

      bot.inlineMenu("Properties 2", properties);
      depth = 1;
    }

    String menu1;
    if (!isMenuProperties && !isMenuProperties_2) {

      if (noticeOnOff) {
        menu1 = F("Check status \n Properties \n Properties 2 \n Notice change On Off \t On");
      } else {
        menu1 = F("Check status \n Properties \n Properties 2 \n Notice change On Off \t Off");
      }

      bot.inlineMenu("Boiler cottage1", menu1);
      depth = 0;
    }

    lastMsg = msg.text;
  }



  void changeStateSend(bool withoutCondition) {
    if (noticeOnOff || withoutCondition) {
      bot.deleteMessage(bot.lastBotMsg());
      checkStatusSend();

      String menu1;
      if (noticeOnOff) {
        menu1 = F("Check status \n Properties \n Properties 2 \n Notice change On Off \t On");
      } else {
        menu1 = F("Check status \n Properties \n Properties 2 \n Notice change On Off \t Off");
      }

      bot.inlineMenu("Boiler cottage1", menu1);
    }
  }

  void sendAlarm(String noticeAlarm) {
    bot.deleteMessage(bot.lastBotMsg());
    bot.sendMessage(noticeAlarm);
    String menu1;
    if (noticeOnOff) {
      menu1 = F("Check status \n Properties \n Properties 2 \n Notice change On Off \t On");
    } else {
      menu1 = F("Check status \n Properties \n Properties 2 \n Notice change On Off \t Off");
    }
    bot.inlineMenu("Boiler cottage1", menu1);
    depth = 0;
  }


  void
  setPtrIsPumpWork(bool *isPumpWork) {
    ptrIsPumpWork = isPumpWork;
  }

  void setPtrIsPumpWork_2(bool *isPumpWork) {
    ptrIsPumpWork_2 = isPumpWork;
  }

  void setPtrTempSensor(int *temp){
    tempSensor = temp;
  }

  void setPtrTempSensor_2(int *temp){
    tempSensor_2 = temp;
  }


};

#endif